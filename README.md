# NekoRay / NekoBox for PC

Поддерживаемый форк **NekoBox for PC** — кроссплатформенный GUI-клиент прокси на базе Qt (бэкенд sing-box).
Оригинальный репозиторий [MatsuriDayo/nekoray](https://github.com/MatsuriDayo/nekoray) архивирован — этот форк продолжает разработку.

A maintained fork of **NekoBox for PC** — Qt based cross-platform GUI proxy configuration manager (backend: sing-box).
The original repository is archived, this fork continues the work.

## Возможности форка / Fork features

- 🌙 **Тёмная тема** — полноценная тёмная тема на базе Fusion-стиля (Настройки → Тема → Dark)
- 📡 **Заголовки запроса подписки в JSON** — задаются индивидуально для каждой подписки и отправляются при каждом обновлении:

  ```json
  {
    "X-HWID": "b4d9f2a1c8e37605",
    "X-Device-OS": "Android",
    "X-Ver-OS": "14",
    "X-Device-model": "Galaxy S24",
    "X-Device-Locale": "en"
  }
  ```

- 🔧 **Индивидуальный User-Agent** для каждой подписки (по умолчанию берётся из глобальных настроек)
- 🏗️ Обновлённая CI-сборка (GitHub Actions v4/v5, актуальные раннеры)

## Прокси / Proxy

- SOCKS (4/4a/5)
- HTTP(S)
- Shadowsocks
- VMess
- VLESS
- Trojan
- TUIC (sing-box)
- NaïveProxy (Custom Core)
- Hysteria2 (Custom Core или sing-box)
- Custom Outbound / Custom Config / Custom Core

## Скачать / Download

### GitHub Releases (Portable ZIP)

Портативная версия без установщика — скачайте архив из [Releases](https://github.com/elizqmill/nekoray/releases), распакуйте и запускайте.

[![Releases](https://img.shields.io/github/v/release/elizqmill/nekoray?logo=github&style=flat-square)](https://github.com/elizqmill/nekoray/releases)

## Windows

Если не хватает DLL — установите [Microsoft Visual C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe).

## Linux

[Инструкция запуска](docs/Run_Linux.md)

## Параметры запуска / Run flags

[docs/RunFlags.md](docs/RunFlags.md)

## Сборка из исходников / Build

[docs/Build_Windows.md](docs/Build_Windows.md) · [docs/Build_Linux.md](docs/Build_Linux.md) · [docs/Build_Core.md](docs/Build_Core.md)

## Roadmap

- [ ] Переход на актуальный sing-box core
- [ ] Экспорт/импорт настроек подписок вместе с заголовками

## Credits

Fork of [MatsuriDayo/nekoray](https://github.com/MatsuriDayo/nekoray).

Core:

- [SagerNet/sing-box](https://github.com/SagerNet/sing-box)
- [Matsuridayo/sing-box-extra](https://github.com/MatsuriDayo/sing-box-extra)
- [XTLS/Xray-core](https://github.com/XTLS/Xray-core) (legacy)

GUI:

- [Qt](https://www.qt.io/)
- [Qv2ray](https://github.com/Qv2ray/Qv2ray)
- [protobuf](https://github.com/protocolbuffers/protobuf)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [zxing-cpp](https://github.com/nu-book/zxing-cpp)
- [QHotkey](https://github.com/Skycoder42/QHotkey)
- [AppImageKit](https://github.com/AppImage/AppImageKit)

## Лицензия / License

[GPLv3](LICENSE)
