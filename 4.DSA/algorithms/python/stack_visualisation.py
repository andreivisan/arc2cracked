#!/usr/bin/env python3

# visualisation of the call stack for adding 2 numbers

import matplotlib.pyplot as plt
import matplotlib.patches as patches

from matplotlib.animation import FuncAnimation


fig, ax = plt.subplots()
ax.set_xlim(0, 1)
ax.set_ylim(0, 10)

class MemBlock:

    def __init__(self, rect, text, label, x_rect, y_rect, y_stop, width, heigth, x_text, y_text):
        self.rect = rect
        self.text = text
        self.label = label
        self.x_rect = x_rect
        self.y_rect = y_rect
        self.y_stop = y_stop
        self.width = width
        self.heigth = heigth
        self.x_text = x_text
        self.y_text = y_text
        self.animation = None

    def create_rect(self):
        self.rect = patches.Rectangle((self.x_rect, self.y_rect), self.width, self.heigth, linewidth=2, edgecolor='g', facecolor='none')
        ax.add_patch(self.rect)
        self.text = ax.text(self.x_text, self.y_text, self.label, horizontalalignment='center', verticalalignment='center', fontsize=12)

    def update(self, frame):
        self.y_rect = 9 - frame / 10
        if self.y_rect < self.y_stop:
            self.y_rect = self.y_stop
            # Stop the animation
            self.animation.event_source.stop()
        self.rect.set_y(self.y_rect)
        self.text.set_y(self.y_rect + 0.5)  # Adjust text position relative to the rectangle


func_param_1_mem_block = MemBlock(None, None, 'Function Variable 1 - a' , 0.2, 9, 0.3, 0.6, 1, 0.5, 9.7)
func_param_1_mem_block.create_rect()
ani = FuncAnimation(fig, func_param_1_mem_block.update, frames=range(101), interval=20)
func_param_1_mem_block.animation = ani

func_param_2_mem_block = MemBlock(None, None, 'Function Variable 2 - b', 0.2, 11, 1.6, 0.6, 1, 0.5, 11.7)
func_param_2_mem_block.create_rect()
ani_2 = FuncAnimation(fig, func_param_2_mem_block.update, frames=range(101), interval=30)
func_param_2_mem_block.animation = ani_2

if __name__ == '__main__':
    plt.show()
