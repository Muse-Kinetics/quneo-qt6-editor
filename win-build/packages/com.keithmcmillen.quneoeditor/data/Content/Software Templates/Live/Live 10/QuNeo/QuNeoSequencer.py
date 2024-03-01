from ConfigurableButtonElement import ConfigurableButtonElement
from _Framework.InputControlElement import *
from _Framework.CompoundComponent import CompoundComponent
from QuNeoUtility import QuNeoUtility

from MIDI_Map import *

class QuNeoSequencer(CompoundComponent, QuNeoUtility):
  """ A Component that's used to drive the step sequencer """

  def __init__(self, matrix, session):
    super(QuNeoSequencer, self).__init__()
    self._matrix = matrix
    self._session = session
    self._slot_launch_button = None
    self._clip_loop_start = None
    self._clip_loop_length = None
    self._slot_step_sequencer_buttons = []
    self._note_up_button = None
    self._note_down_button = None
    self._measure_left_button = None
    self._measure_right_button = None
    self._sequencer_clip = None

    self._buttons = []
    self.scale = CHROMATIC_SCALE
    self.notes = []
    self.key_offset = 7
    self._key_index = 36
    self._loc_offset = 0.0
    self.grid_size = 2.0
    self.loop_up_table = 8
    self._clip_slot = None
    self._clip_notes = None

    self.song().view.add_detail_clip_listener(self.on_clip_changed)
    self.song().add_current_song_time_listener(self.force_leds)

  def force_leds(self):
    pass

  def button(self, channel, note, color = GREEN_HI):
    return ConfigurableButtonElement(True, MIDI_NOTE_TYPE, channel, note, color)

  def create_note(self, value, loc):
    if (value):
      x = (value, loc, 0.25, 127, False)
    else:
      None
    return x

  def setup(self, as_active = True):
    if as_active:
      self.set_movement_buttons()      
      self.bind_to_matrix()
      self.clear_led()
      self.update()
      self.on_device_changed()
    else:
      self.bind_to_matrix(False)
      self.set_movement_buttons(False)      

  def set_movement_buttons(self, as_active = True):
    """ Attaches event handlers to the movement buttons """
    if as_active:
      self.set_measure_offset(
          self.button(PAD_CHANNEL, SESSION_LEFT), 
          self.button(PAD_CHANNEL, SESSION_RIGHT))
      self.set_note_offset(
          self.button(PAD_CHANNEL, SESSION_UP), 
          self.button(PAD_CHANNEL, SESSION_DOWN))
    else:
      self.set_measure_offset(None, None)
      self.set_note_offset(None, None)

  def set_slot_launch_button(self, button):
    if self._slot_launch_button is not button:
      if self._slot_launch_button is not None:
        self._slot_launch_button.remove_value_listener(self._slot_launch_value)

      self._slot_launch_button = button
      if (self._slot_launch_button != None):
        self._slot_launch_button.add_value_listener(self._slot_launch_value)
      self.update()

  def set_note_offset(self, up_button, down_button):
    """ Up and down movement buttons """
    if self._note_up_button is not None:
      self._note_up_button.remove_value_listener(self._note_up_value)
    self._note_up_button = up_button
    if self._note_up_button is not None:
      self._note_up_button.add_value_listener(self._note_up_value)

    if self._note_down_button is not None:
      self._note_down_button.remove_value_listener(self._note_down_value)
    self._note_down_button = down_button
    if self._note_down_button is not None:
      self._note_down_button.add_value_listener(self._note_down_value)

  def set_measure_offset(self, left_button, right_button):
    """ Left and right movement buttons """
    if self._measure_left_button is not None:
      self._measure_left_button.remove_value_listener(self._measure_left)
    self._measure_left_button = left_button
    if self._measure_left_button is not None:
      self._measure_left_button.add_value_listener(self._measure_left)

    if self._measure_right_button is not None:
      self._measure_right_button.remove_value_listener(self._measure_right)
    self._measure_right_button = right_button
    if self._measure_right_button is not None:
      self._measure_right_button.add_value_listener(self._measure_right)

  def clear_led(self):
    if (self._slot_step_sequencer_buttons != None):
      for button in self._slot_step_sequencer_buttons:
        button.count = 0
        button.note_on()


  def update_key_index(self, value):
    if (value != None):
      new_value = value
      if (new_value < 0.0 and new_value > 127.0):
        new_value = 0.0
      elif (new_value > 127.0):
        new_value = 127.0
      else:
        new_value
      self._key_index = new_value
      if (self._slot_step_sequencer_buttons != None):
        for button in self._slot_step_sequencer_buttons:
          button.count = 0
          button.note_on()
      self.clear_led()
      self.update_notes()

  def update_measure_offset(self, value):
    new_value = value
    if (new_value < 0.0):
      new_value = 0.0
    elif (new_value > 127.0):
      new_value = 127.0
    else:
      new_value
    self._loc_offset = new_value
    self.clear_led()
    self.update_notes()


  def update(self):
    if (self.song().view.highlighted_clip_slot != None):
      self._sequencer_clip = self.song().view.highlighted_clip_slot.clip
    else:
      self._sequencer_clip = None
    self.update_notes()


  def _measure_left(self, value):
    assert (value in range(128))
    assert (self._measure_left_button != None)
    if self.is_enabled():
      if (value != 0):
        new_key_index = -self.grid_size
        real_key_index = (new_key_index + self._loc_offset)
        self.update_measure_offset(real_key_index)
      else:
        None

  def _measure_right(self, value):
    assert (value in range(128))
    assert (self._measure_right_button != None)
    if self.is_enabled():
      if (value != 0):
        new_key_index = self.grid_size
        real_key_index = (new_key_index + self._loc_offset)
        self.update_measure_offset(real_key_index)
      else:
        None

  def _note_up_value(self, value):
    assert (value in range(128))
    assert (self._note_up_button != None)
    if (self.is_enabled()):
      if (value != 0):
        new_key_index = -self.key_offset
        real_key_index = (new_key_index + self._key_index)
        if (real_key_index < 0):
          real_key_index = 127
        self.update_key_index(real_key_index)
      else:
        None

  def _note_down_value(self, value):
    assert (value in range(128))
    assert (self._note_down_button != None)
    if (self.is_enabled()):
      if (value != 0):
        new_key_index = self.key_offset
        real_key_index = (new_key_index + self._key_index)
        if (real_key_index > 127):
          real_key_index = 0
        self.update_key_index(real_key_index)
      else:
        None

  def set_clip_loop_start(self, slider):
    assert ((slider == None) or isinstance(slider, EncoderElement))
    if(self._clip_loop_start != slider):
      if (self._clip_loop_start != None):
        self._clip_loop_start.remove_value_listener(self._slot_launch_loop_value)
      self._clip_loop_start = slider
      if (self._clip_loop_start != None):
        self._clip_loop_start.add_value_listener(self._slot_launch_loop_value)
      self.update()

  def set_clip_loop_length(self, slider):
    assert ((slider == None) or isinstance(slider, EncoderElement))
    if(self._clip_loop_length != slider):
      if (self._clip_loop_length != None):
        self._clip_loop_length.remove_value_listener(self._clip_loop_length_value)
      self._clip_loop_length = slider
      if (self._clip_loop_length != None):
        self._clip_loop_length.add_value_listener(self._clip_loop_length_value)
      self.update()

  """ Attaches our note creation/etc callbacks to the MATRIX """
  def bind_to_matrix(self, as_active = True):
    if as_active:
      buttons = []
      for button, (x, y) in self._matrix.iterbuttons():
        buttons.append(button)
      self.set_sequencer_buttons(buttons)
    else:
      self.set_sequencer_buttons(None)

  def set_sequencer_buttons(self, buttons):
    if (self._slot_step_sequencer_buttons != buttons):
      if (self._slot_step_sequencer_buttons != None):
        for button in self._slot_step_sequencer_buttons:
          button.remove_value_listener(self._slot_step_sequencer_value)
      self._slot_step_sequencer_buttons = buttons
      if (self._slot_step_sequencer_buttons != None):
        for button in self._slot_step_sequencer_buttons:
          button.add_value_listener(self._slot_step_sequencer_value, identify_sender=True)

  def update_buttons(self):
    for row in range(64):
      for index in range(len(self.new_clip_notes)):
        if self.new_table[row][1] == self.new_clip_notes[index][0] and self.new_table[row][2] == self.new_clip_notes[index][1]:
          if (self._slot_step_sequencer_buttons != None):
            for note in self._slot_step_sequencer_buttons:
              if (note._note == self.new_table[row][0]):
                note.count = 1
                note.note_on()
          else:
            None

  """ 
  Loc offset == location offset?
  Builds table of three values:
    * button MIDI NOTE
    * Key / vertical position on MIDI grid
    * Location / horizontal position on MIDI grid
  """ 
  def update_quneo_matrix(self):
    self.new_table = []
    for row in range(8):
      for col in range(8):
        self.new_table.append([CLIP_NOTE_MAP[row][col], (self._key_index+self.scale[row]), (self._loc_offset+col/4.0)])

    self.new_clip_notes = []
    if self._clip_notes != None:
      for note in self._clip_notes:
        if note[0] >= self._key_index and note[0] < (self._key_index+self.loop_up_table) and note[1] >= self._loc_offset and note[1] < (self._loc_offset+self.grid_size):
          self.new_clip_notes.append([note[0], note[1]])
    self.update_buttons()

  def update_notes(self):
    if self._sequencer_clip != None:
      if self._sequencer_clip.is_midi_clip:
        self._sequencer_clip.select_all_notes()
        note_cache = list(self._sequencer_clip.get_selected_notes())
        self._sequencer_clip.deselect_all_notes()
        if self._clip_notes != note_cache:
          self._clip_notes = note_cache

      self.update_quneo_matrix()

  def _slot_launch_value(self, value):
    assert (value in range(128))
    assert (self._slot_launch_button != None)
    if self.is_enabled():
      if ((value != 0) or (not self._slot_launch_button.is_momentary())):
        if (self.song().view.highlighted_clip_slot != None):
          self.song().view.highlighted_clip_slot.fire()

  def _slot_launch_loop_value(self, value):
  	assert (value in range(128))
  	assert (self._clip_loop_start != None)
  	if self.is_enabled():
  		if value != 0:
  			if value > 1:
  				new_value = -0.25
  			else:
  				new_value = 0.25
  			if (self.song().view.highlighted_clip_slot != None):
  				if self.song().view.highlighted_clip_slot.clip != None:
  					if(self.song().view.highlighted_clip_slot.clip.looping != False):
  						self.song().view.highlighted_clip_slot.clip.add_loop_start_listener
  						loop_start_pos = self.song().view.highlighted_clip_slot.clip.loop_start
  						loop_end_pos = self.song().view.highlighted_clip_slot.clip.loop_end
  						loop_length = self.song().view.highlighted_clip_slot.clip.length
  						real_value = (loop_start_pos + new_value)
  						if real_value < 0.0:
  							None
  						else:
  							self.song().view.highlighted_clip_slot.clip.loop_start = real_value
  				else:
  					None

  def _clip_loop_length_value(self, value):
  	assert (value in range(128))
  	assert (self._clip_loop_length != None)
  	if self.is_enabled():
  		if value != 0:
  			if value > 1:
  				new_value = -0.25
  			else:
  				new_value = 0.25
  			if (self.song().view.highlighted_clip_slot != None):
  				if (self.song().view.highlighted_clip_slot.clip != None):
  					if(self.song().view.highlighted_clip_slot.clip.looping != False):
  						self.song().view.highlighted_clip_slot.clip.add_loop_end_listener
  						loop_end_pos = self.song().view.highlighted_clip_slot.clip.loop_end
  						loop_length = self.song().view.highlighted_clip_slot.clip.length
  						real_value = (loop_end_pos + new_value)
  						if real_value < 0.25:
  							return None
  						else:
  							self.song().view.highlighted_clip_slot.clip.loop_end = real_value
  				else:
  					None
  						
  """ Called when a button the matrix is pressed """
  def _slot_step_sequencer_value(self, value, sender):
  	if self.is_enabled():
  		if self._sequencer_clip != None and self._sequencer_clip.is_midi_clip:
  			if value != 0:
  				for row in range(8):
  					for col in range(8):
  						if (sender._note == STEP_SEQUENCER_MAP[row][col]):
  							sender.count += 1
  							sender.note_on()
  							note_value = self._key_index + self.scale[row]
  							loc_value = self._loc_offset + (col/4.0)
  							if (self._clip_notes != None):
  								if (sender.count == 1):
  									self._clip_notes.append(self.create_note(note_value, loc_value))
  								else:
  									for note in self._clip_notes:
  										if (note_value == note[0] and loc_value == note[1]):
  											self._clip_notes.remove(note)
  								self._sequencer_clip.select_all_notes()
  								self._sequencer_clip.replace_selected_notes(tuple(self._clip_notes))
  							else:
  								None
  	self.update_notes()

  def set_drum_pad_mode(self, value):
    if (value == 0):
      self._parent.log_message("!ST VALUE "+str(value))
    elif (value == 1):
      self._parent.log_message("2ND VALUE "+str(value))
    else:
      None

  def on_selected_track_changed(self):
    if (self._slot_step_sequencer_buttons != None):
      for button in self._slot_step_sequencer_buttons:
        button.count = 0
        button.note_on()
    self.on_device_changed()
    self.update()
    self.update_notes()

  def on_device_changed(self):
    if (self.song().view.selected_track.view.selected_device != None):
      self.device = self.song().view.selected_track.view.selected_device
      self.device_name = self.device.class_name
      if (self.device_name == 'InstrumentImpulse'):
        self._key_index = 60
        self.scale = MAJOR_SCALE
        self.key_offset = 0
        self.loop_up_table = 13
      elif (self.device_name == 'DrumGroupDevice'):
        self._key_index = 36
        self.scale = CHROMATIC_SCALE
        self.key_offset = 7
        self.loop_up_table = 8
      elif (self.device_name == 'Collision'):
        self._key_index = 36
        self.scale = CHROMATIC_SCALE
        self.key_offset = 7
        self.loop_up_table = 8
      else:
        self.scale = CHROMATIC_SCALE
        self._key_index = 36
        self.key_offset = 7
        self.loop_up_table = 8
    self.update()

  def on_clip_changed(self):
    # Can we do this better?
    #self.setup(False)
    #self.setup()
    self.clear_led()
    self.on_device_changed()

  def disconnect(self):
    super(QuNeoSequencer, self).disconnect() 
    self.song().remove_current_song_time_listener(self.force_leds)
    self.song().view.remove_detail_clip_listener(self.on_clip_changed)
