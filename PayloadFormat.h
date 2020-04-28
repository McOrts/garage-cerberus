function Decoder(bytes, port) {
  var text = String.fromCharCode.apply(null, bytes);
  return {
    sensor_status: text
  };
}