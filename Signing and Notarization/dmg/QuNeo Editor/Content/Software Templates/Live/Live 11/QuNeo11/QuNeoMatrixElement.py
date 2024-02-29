from _Framework.ButtonMatrixElement import ButtonMatrixElement

class QuNeoMatrixElement(ButtonMatrixElement):
  """ Custom matrix element that allows us to do full resets """

  def clear(self):
    for row in range(self.height()):
      for column in range(self.width()):
        self.get_button(column, row).turn_off()
