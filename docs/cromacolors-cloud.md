# Cromacolors Cloud

The Cromacolors app uses the existing native cloud agent and can be pointed to a
custom backend through `AppConfig`.

## Client Configuration

Set these keys in the application config:

```ini
cromacolors_api_url=claud.chromacolor.shop
cromacolors_auth_url=https://claud.chromacolor.shop
cromacolors_cloud_url=https://claud.chromacolor.shop
cromacolors_pub_key=your_publishable_client_key
```

The old `orca_*` keys are still accepted as fallback for compatibility.

## Included Server

This repository now includes a first ChromaColors Cloud backend in:

```text
cloud-server/server.py
```

Run it with:

```bash
cd cloud-server
CHROMA_SECRET="replace-with-a-long-random-secret" python3 server.py
```

It provides the login page, PKCE token exchange, refresh token, logout, health,
and basic sync endpoints. See `cloud-server/README.md` for deploy notes and an
Nginx proxy example for `https://claud.chromacolor.shop`.

For Ubuntu 25.04 deployment, use:

```text
cloud-server/deploy/README-ubuntu.md
cloud-server/deploy/install_ubuntu.sh
```

## Required Login Page

The slicer opens:

```text
https://claud.chromacolor.shop/cromacolors-login?lang=<language>
```

That page must communicate with the embedded WebView by posting the same
commands used by the Orca flow:

- Request login config: `{"command":"get_login_cmd"}`
- Complete login: `{"command":"user_login","data":{"code":"...","state":"..."}}`
- Optional external browser login: `{"command":"thirdparty_login","data":{"url":"..."}}`

The app provides PKCE data back to the page through `window.postMessage(...)`
with:

- `backend_url`
- `apikey`
- `pkce.code_challenge`
- `pkce.code_challenge_method`
- `pkce.state`
- `pkce.redirect_uri`
- `pkce.loopback_port`

## Required API Endpoints

The current client expects an Orca-compatible API surface:

- `GET /api/v1/health`
- `POST /auth/v1/token?grant_type=pkce`
- `POST /auth/v1/token?grant_type=refresh_token`
- `POST /auth/v1/logout`
- `POST /api/v1/sync/pull`
- `POST /api/v1/sync/push`
- `POST /api/v1/sync/delete`
- `GET /api/v1/sync/profiles`
- `GET /api/v1/subscriptions`

For a first working version, implement auth plus health first. Preset sync and
profile sharing can be added after login is stable.
