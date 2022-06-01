import Live

from _Framework.SliderElement import *

class ConfigurableSliderElement(SliderElement):
    __module__ = __name__
    __doc__ = ' Special Slider class that can be configured with custom on- and off-values '

    def __init__(self, msg_type, channel, identifier):
        ButtonElement.__init__(self, msg_type, channel, identifier)

    def send_value(self,value):
    	if(value != self._last_active_value):
            ButtonElement.send_value(self, value, (force or self._force_next_value))
        self._last_active_value = value
        self._force_next_value = False