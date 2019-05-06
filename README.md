Ubinder is library for creating bindings on various languages.

You specify the command-oriented protocol for communication with your library (typically with [protobuf](https://developers.google.com/protocol-buffers/), but can be any binary format).
All other thing is done for you by ubinder.

To create a binding for all supported hosts you need to wrap you library and expose function from [include/ubinder/wrapper_interface.h](https://github.com/teams2ua/ubinder/blob/master/include/ubinder/wrapper_interface.h)


## Supported hosts

* Node.js

## Planned hosts

* React Native
* Java
* Python

## Feedback

If you have any comments or feedback, feel free to [open an issue](https://github.com/teams2ua/ubinder)