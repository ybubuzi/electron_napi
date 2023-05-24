const binding = require('bindings')(`Counter`)
module.exports = {
  counter: binding.counter
}
