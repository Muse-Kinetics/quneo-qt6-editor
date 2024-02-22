from ConfigurableButtonElement import ConfigurableButtonElement
from _Framework.InputControlElement import *
from _Framework.SliderElement import SliderElement
from MIDI_Map import *

class QuNeoUtility(object):
  """ Provides some functionality shared across ALL classes """

  def button(self, channel, note, color = GREEN_HI):
    return ConfigurableButtonElement(True, MIDI_NOTE_TYPE, channel, note, color)

  def encoder(self, channel, cc):
    return SliderElement(MIDI_CC_TYPE, channel, cc)

