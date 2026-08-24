package main

import (
	"context"
	"net"
	"net/http"

	"github.com/matsuridayo/libneko/neko_common"
	"github.com/matsuridayo/libneko/neko_log"
	box "github.com/sagernet/sing-box"
	"github.com/sagernet/sing-box/adapter"
	"github.com/sagernet/sing-box/boxapi"
	"github.com/sagernet/sing-box/common/dialer"
	"github.com/sagernet/sing-box/experimental/deprecated"
	"github.com/sagernet/sing-box/include"
	"github.com/sagernet/sing-box/log"
	"github.com/sagernet/sing-box/option"
	"github.com/sagernet/sing/common/json"
	M "github.com/sagernet/sing/common/metadata"
	"github.com/sagernet/sing/service"
)

var instance *box.Box
var instance_cancel context.CancelFunc

// nekoStats counts traffic for connections dialed through the boxapi helpers.
// It is created when the main instance starts (see grpc_box.go).
var nekoStats *boxapi.SbV2rayServer

func statsTrackerFor(i *box.Box) adapter.ConnectionTracker {
	if i == instance && nekoStats != nil {
		return nekoStats.StatsService()
	}
	return nil
}

// createBoxInstance replaces the old boxmain.Create helper removed from the
// sing-box 1.12 fork: parse config JSON, disable colored log output,
// then create and start the instance.
func createBoxInstance(configContent []byte) (*box.Box, context.CancelFunc, error) {
	ctx := service.ContextWith(context.Background(), deprecated.NewStderrManager(log.StdLogger()))
	ctx = include.Context(ctx)

	options, err := json.UnmarshalExtendedContext[option.Options](ctx, configContent)
	if err != nil {
		return nil, nil, err
	}
	if options.Log == nil {
		options.Log = &option.LogOptions{}
	}
	options.Log.DisableColor = true

	ctx, cancel := context.WithCancel(ctx)
	b, err := box.New(box.Options{
		Context: ctx,
		Options: options,
	})
	if err != nil {
		cancel()
		return nil, nil, err
	}
	if err = b.Start(); err != nil {
		cancel()
		return nil, nil, err
	}
	return b, cancel, nil
}

func setupCore() {
	//
	neko_log.SetupLog(50*1024, "./neko.log")
	//
	neko_common.GetCurrentInstance = func() interface{} {
		return instance
	}
	neko_common.DialContext = func(ctx context.Context, specifiedInstance interface{}, network, addr string) (net.Conn, error) {
		i, ok := specifiedInstance.(*box.Box)
		if !ok || i == nil {
			i = instance
		}
		if i != nil {
			return boxapi.DialContext(ctx, i, statsTrackerFor(i), network, addr)
		}
		return neko_common.DialContextSystem(ctx, network, addr)
	}
	neko_common.DialUDP = func(ctx context.Context, specifiedInstance interface{}) (net.PacketConn, error) {
		i, ok := specifiedInstance.(*box.Box)
		if !ok || i == nil {
			i = instance
		}
		if i != nil {
			return dialer.NewDetour(i.Outbound(), i.Outbound().Default().Tag(), true).ListenPacket(ctx, M.Socksaddr{})
		}
		return neko_common.DialUDPSystem(ctx)
	}
	neko_common.CreateProxyHttpClient = func(specifiedInstance interface{}) *http.Client {
		i, _ := specifiedInstance.(*box.Box)
		return boxapi.CreateProxyHttpClient(i, statsTrackerFor(i))
	}
}
