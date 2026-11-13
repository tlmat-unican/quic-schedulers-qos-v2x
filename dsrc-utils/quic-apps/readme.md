# GOLANG APPS

## Requirements 
-  The `quic-go` version used in this project is only compatible with Go releases up to 1.20.  
  Using a newer Go version may cause compilation errors or unexpected behavior.
- 

## System configuration

```bash
sudo sysctl -w net.core.rmem_max=2500000
sudo sysctl -w net.core.rmem_default=2500000
```


## To build go_client app
```bash
go build client_OBU.go traffic_gen.go 
```


## To build go_server app
```bash
go build server.go
```


# APPS funcionalities

- Go-apps can be used with both TCP protocol and also quic
- To select the protocol make use of the TCP field in the `json` files

:warning: When using the TCP protocol:
1. Ensure the [`UDP tunnel`](../udptunnel/) is running.
2. Bring up the TUN interfaces.