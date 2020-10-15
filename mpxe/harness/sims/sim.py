from mpxe.harness.protogen import stores_pb2
from mpxe.harness.protogen import gpio_pb2

class Sim:
    def handle_update(self, pm, proj):
        pass

    def handle_log(self, pm, proj, log):
        print('[{}]'.format(proj.name), log)

    def set_gpio(self, proj, port, pin, state):
        ind = (ord(port.capitalize()) - ord('A')) * 16 + pin
        gpio_msg = gpio_pb2.MxGpioStore()
        gpio_msg.state.extend([0] * 6 * 16)
        gpio_msg.state[ind] = state
        gpio_mask = gpio_pb2.MxGpioStore()
        gpio_mask.state.extend([0] * 6 * 16)
        gpio_mask.state[ind] = state
        proj.write_store(stores_pb2.MxStoreType.GPIO, gpio_msg, gpio_mask)