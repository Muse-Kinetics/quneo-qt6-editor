from _Framework.ModeSelectorComponent import ModeSelectorComponent
from MIDI_Map import *

class SubSelectorComponent(ModeSelectorComponent):
  """ Subselector that switches between session and sequencer modes """

  def __init__(self, matrix, session, mixer, sequencer, parent): 
    ModeSelectorComponent.__init__(self)
    self._matrix = matrix
    self._session = session
    self._mixer = mixer
    self._parent = parent
    self._sequencer = sequencer
    self.set_mode(0)

  def number_of_modes(self):
    return 2

  # Returns true when the root mode selector is engaged: e.g. in mode switch mode
  def mode_selektor_engaged(self):
    return self._parent.mode() == 1

  def update(self):
    # If the parent selector is engaged we only render our 
    # mode selector buttons 
    if self.mode_selektor_engaged(): 
      self._session.setup(False)
      self._mixer.setup_matrix_controls(False)
      self._sequencer.setup(False)
      self._setup_mode_buttons()

    # Otherwise we look at our current mode, and render either
    # Session View (mode 0), or 
    # Sequencer View (mode 1)
    else:
      self._setup_mode_buttons(False)
      if self.mode() == 0:
        self._sequencer.setup(False)
        self._session.setup()
        self._mixer.setup_matrix_controls()
      else:
        self._matrix.clear()
        self._session.setup(False)
        self._mixer.setup_matrix_controls(False)
        self._sequencer.setup()

  def _setup_mode_buttons(self, as_active = True):
    mode_buttons = []
    for button_index in range(self.number_of_modes()):
      mode_buttons.append(self._matrix.get_button(button_index, (self._matrix.height() - 1)))
    if as_active:
      self.set_mode_buttons(mode_buttons)
      self._matrix.clear()
      for index, button in enumerate(mode_buttons):
        if self.mode() == index:
          button.send_value(GREEN_HI)
        else:
          button.send_value(RED_HI)
    else:
      self.set_mode_buttons(None)

  def set_mode_buttons(self, buttons):
    if buttons is None:
      for button in self._modes_buttons:
        button.remove_value_listener(self._mode_value)
      self._modes_buttons = []
    else:
      for button in buttons:
        identify_sender = True
        button.add_value_listener(self._mode_value, identify_sender)
        self._modes_buttons.append(button)
    # pick either session or sequencer

  def mode(self):
    return self._mode_index
