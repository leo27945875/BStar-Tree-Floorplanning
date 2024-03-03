import os
import sys
import math
import matplotlib.pyplot as plt
import matplotlib.patches as patches

blocks = []
area = 0
ratio = 0
width = 0
height = 0

class Block:
    def __init__(self, b_id, b_x, b_y, b_width, b_height):
        self.b_id = b_id
        self.b_x = b_x
        self.b_y = b_y
        self.b_width = b_width
        self.b_height = b_height

def parse_blocksize(f):
    for i, line in enumerate(f):
        if i > 1:
            block = line.split()
            b_id = block[0]
            b_x = block[1]
            b_y = block[2]
            b_width = block[3]
            b_height = block[4]
            blocks.append(Block(b_id, b_x, b_y, b_width, b_height))
        elif i == 0:
            global area 
            area = int(line.split()[2])
        elif i == 1:
            global ratio 
            ratio = float(line.split()[2])
    
    global width 
    width = math.sqrt(area * ratio)
    global height 
    height = area / width

def plot():
    fig, ax = plt.subplots()
    ax.set_xlim(-10, width + 10)
    ax.set_ylim(-10, height + 10)
    ax.set_xlabel('X Axis')
    ax.set_ylabel('Y Axis')
    ax.xaxis.grid(True, linestyle='--', linewidth=0.5, color='gray')
    ax.yaxis.grid(True, linestyle='--', linewidth=0.5, color='gray')
    for block in blocks:
        b_x, b_y, b_width, b_height = float(block.b_x), float(block.b_y), float(block.b_width), float(block.b_height)
        rect = patches.Rectangle((b_x, b_y), b_width, b_height, linewidth=1, edgecolor='black', facecolor='blue', alpha=0.5)
        ax.add_patch(rect)

        label_0 = f'{block.b_id}\n{int(b_width)},{int(b_height)}'
        # ax.text(b_x + b_width/2, b_y + b_height/2, label_0, fontsize=12, ha='center', va='center', color='white')
        label_1 = f'({int(b_x)},{int(b_y)})'
        # ax.text(b_x , b_y, label_1, fontsize=12, ha='left', va='bottom')
        ax.scatter(b_x, b_y, marker='*', color='red', s=100, label='Left Bottom')

    fig.savefig(os.path.join(os.path.dirname(__file__), 'figures', 'plot.png'))

if __name__ == '__main__':

    debug_file = "./data/DEBUG.txt"

    with open(debug_file, 'r') as f:
        parse_blocksize(f)

    plot()

    print("Done!")
    sys.exit(0)

    