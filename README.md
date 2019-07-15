# Ubinder - universal binder

Ubinder is library for creating asynchronous bindings on various languages/environments.

You specify the command-oriented protocol for communication with your library (typically with [protobuf](https://developers.google.com/protocol-buffers/), but can be any binary format).
All other thing is done for you by ubinder.

<p align="center">
 <img src="/docs/images/ubinder_overview.png" width="550"/>
</p>

Ubinder implement two types of protocol:
* Request-Response
* Notification

## Supported hosts

* Node.js [How to use](./src/node/README.md)
* React Native (Android)
* Java

## Planned hosts

* React Native (IOS)
* Python


## [How to create wrapping library](./HowToWrap.md)

## Feedback

If you have any comments or feedback, feel free to [open an issue](https://github.com/teams2ua/ubinder)