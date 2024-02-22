from _Framework.MixerComponent import MixerComponent
from QuNeoUtility import QuNeoUtility
from MIDI_Map import *

class QuNeoMixer(MixerComponent, QuNeoUtility):
  def __init__(self, matrix):
    MixerComponent.__init__(self, 8)
    self.sends = []
    self._matrix = matrix
    self.setup()

  def setup_matrix_controls(self, as_active = True):
    for track in range(7):
      strip = self.channel_strip(track)
      if as_active:
        strip.set_mute_button(self._matrix.get_button(track, 5))
        strip.set_solo_button(self._matrix.get_button(track, 6))
        strip.set_arm_button(self._matrix.get_button(track, 7))
        strip._mute_button._on_value = 0 
        strip._mute_button._off_value = GREEN_HI 
        strip._solo_button._on_value = ORANGE_HI 
        strip._arm_button._on_value = ORANGE_HI 
        strip.update()
      else:
        if strip._mute_button:
          strip._mute_button._on_value = RED_HI 
          strip._mute_button._off_value = 0 
          strip.set_mute_button(None)
        if strip._solo_button:
          strip._solo_button._on_value = RED_HI 
        if strip._arm_button:
          strip._arm_button._on_value = RED_HI 
        strip.set_select_button(None) 
        strip.set_solo_button(None) 
        strip.set_arm_button(None) 

  def setup(self, as_active = True):
    self.setup_matrix_controls()
    if as_active:
      self.set_crossfader_control(self.encoder(PAD_CHANNEL, CROSSFADER))
      for track in range(8):
        strip = self.channel_strip(track) 
        if track < 4:
          strip.set_volume_control(self.encoder(SLIDER_CHANNEL, TRACK_VOL[track]))

      for index in range(2):
        self.sends.append(self.encoder(SLIDER_CHANNEL, SELECTED_SENDS[index]))
      self.selected_strip().set_send_controls(tuple(self.sends))
      self.selected_strip().set_pan_control(self.encoder(SLIDER_CHANNEL, SELECTED_PAN))
      self.selected_strip().set_volume_control(self.encoder(SLIDER_CHANNEL, SELECTED_VOL))

      self.set_select_buttons(
          self.button(PAD_CHANNEL, TRACK_RIGHT),
          self.button(PAD_CHANNEL, TRACK_LEFT))
    else:
      self.set_crossfader_control(None)
      for track in range(8):
        strip = self.channel_strip(track) 
        if track < 4:
          strip.set_volume_control(None)
      self.sends = []
      self.selected_strip().set_send_controls(tuple(self.sends))
      self.selected_strip().set_pan_control(None)
      self.selected_strip().set_volume_control(None)
      self.set_select_buttons(None, None)
      self.set_bank_buttons(None, None)

