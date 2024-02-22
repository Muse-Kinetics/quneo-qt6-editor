from _Framework.TransportComponent import TransportComponent
from QuNeoUtility import QuNeoUtility
from MIDI_Map import *

class QuNeoTransport(TransportComponent, QuNeoUtility):

  def __init__(self):
    TransportComponent.__init__(self)
    
    self._tempo_up_button = None
    self._tempo_down_button = None

    self.set_play_button(self.button(PAD_CHANNEL, PLAY))
    self.set_record_button(self.button(PAD_CHANNEL, REC))
    self.set_stop_button(self.button(PAD_CHANNEL, STOP))
    self.set_overdub_button(self.button(PAD_CHANNEL, OVERDUB))
    self.set_metronome_button(self.button(PAD_CHANNEL, METRONOME))
    self.set_tempo_buttons(
        self.button(PAD_CHANNEL, TEMPO_UP), 
        self.button(PAD_CHANNEL, TEMPO_DOWN))
    
  def set_tempo_buttons(self, up_button, down_button):
    if (self._tempo_up_button != None):
        self._tempo_up_button.remove_value_listener(self._tempo_up_value)
    self._tempo_up_button = up_button

    if (self._tempo_up_button != None):
        self._tempo_up_button.add_value_listener(self._tempo_up_value)
    if (self._tempo_down_button != None):
        self._tempo_down_button.remove_value_listener(self._tempo_down_value)
    self._tempo_down_button = down_button

    if (self._tempo_down_button != None):
        self._tempo_down_button.add_value_listener(self._tempo_down_value)
    self.update()

  def _tempo_up_value(self, value):
    if (value != 0):
      new_tempo = 1.0
      real_tempo = (new_tempo + self.song().tempo)
      if real_tempo < 20.0:
        real_tempo = 20.0
      self.update_tempo(real_tempo)
    else:
      None

  def _tempo_down_value(self, value):
    if (value != 0):
      new_tempo = -1.0
      real_tempo = (new_tempo + self.song().tempo)
      if (real_tempo > 200.0):
        real_tempo = 200.0
      self.update_tempo(real_tempo)
    else:
      None


  def update_tempo(self, value):
    if (value != None):
      new_tempo = value
      self.song().tempo = new_tempo
