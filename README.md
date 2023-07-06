# Remo nano Dash Button

押すと Nature Remo nano の OnOff Cluster がトグルするボタンです。

動かし方やコードの解説はこちら。

https://engineering.nature.global/entry/matter-remo-nano-dash-button

ノード ID が `1` で Endpoint ID が `2` の OnOff Cluster をトグルします。
ノード ID と Endpoint ID を変更したい場合は、 `main/app_main.cpp` の以下の部分を変更してください。

```cpp
static constexpr uint32_t TARGET_NODE_ID = 1;
static constexpr uint16_t TARGET_ENDPOINT_ID = 2;
```

## License

Licensed under either of:

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or
  http://www.apache.org/licenses/LICENSE-2.0)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)

at your option.

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the
work by you, as defined in the Apache-2.0 license, shall be dual licensed as above, without any
additional terms or conditions.
