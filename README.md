Ubinder is library for creating bindings to libraries on various languages.

You specify the command-oriented protocol for communication with your library (typically with [pprotobuf](https://developers.google.com/protocol-buffers/), but can be any binary format).
All other thing is done for you by ubinder.

## Why not gRPC/nng or any other messaging library with inProc support?

Good question, glad you ask. Main reason - too many features, bigger API, that makes it harder to add support for new language/platform for binding.
UBinder has very limited number (4) of functions to reimplement in order to support new target.

## Feedback

If you have any comments or feedback, feel free to [open an issue](https://github.com/teams2ua/ubinder)