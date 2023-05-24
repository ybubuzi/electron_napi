const binding = require('bindings')(`Counter`)
const counter = binding.counter
console.log(`=====`, counter('D:/project/demos/napi_call_dll/extension/counter/test/Counter.dll'))
