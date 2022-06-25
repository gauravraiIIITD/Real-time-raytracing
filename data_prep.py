# Import libraries
from turtle import position
import cv2
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines
from matplotlib.patches import Polygon
import matplotlib.image as mpimg
from matplotlib.widgets import Slider, Button
from matplotlib.patches import Circle
import numpy as np
import ipdb
from time import sleep
from Maurice import *


coordinates = []
coords = []
current_segment = []
all_segment = []
flag_move = False
drag_status = False

img = mpimg.imread('maurice.png')
fig = plt.figure()
ax= fig.add_subplot(111)
#ax = plt.gca()
ax.set_xlim(0, 300)
ax.set_ylim(0, 300)
ax.imshow(img, extent=(0, 250, 50, 300))
plt.axis('off')

# Create axes for frequency and amplitude sliders
slider = plt.axes([0.20, 0.95, 0.65, 0.03])


# Create a slider from 0.0 to 20.0 in axes axfreq
# with 3 as initial value
frame = Slider(slider, 'Slider', 0.0, 100.0, 0)
g_seg_index = None
g_pt_index = None


def nFrame():
  print("nFrame Action")

def Save():
  print("Save Action")

def Clear():
    print("Clear the Handle Action")


def reset_view():
    global fig 
    global ax
    
    ax = fig.add_subplot(111)


    #ax = plt.gca()
    ax.set_xlim(0, 300)
    ax.set_ylim(0, 300)
    ax.imshow(img, extent=(0, 250, 50, 300))
    

def onclick(event):
    global all_segment
    if len(all_segment):
        print('onclick $$$$$$$$$$$$$$$$$$$$$$', len(all_segment[0]))
    global flag_move
    print(flag_move)
    if not flag_move:
        print('inside Called @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@')
        global ix, iy
        
        ix, iy = event.xdata, event.ydata
        bounds_btn = (0.12, 0.73, 0.15, 0.23)

        if ix is not None and iy is not None and (ix < 0.12 or ix > 0.73) and (iy >0.23):
            print('calling on click @@@@@@@@@', event.inaxes)
            print('x = %d, y = %d'%(ix, iy))
            
            #plt.plot(ix, iy, color="white", linewidth=3)
            global coords
            print('#####', ix, iy)
            coords.append([ix, iy])
            #print(coords)
            global ax
            coords_array=np.array(coords)
            if len(all_segment) >= 1 and len(coords):
                all_segment[-1] = coords_array
            else:
                if len(coords):
                    all_segment.append(coords_array)
            #print(coords_array.shape)
            for segment in all_segment:
                try:
                    ax.plot(segment[:,0], segment[:,1], markersize=10)
                    ax.scatter(segment[:,0], segment[:,1] )
                except IndexError as e:
                    print('error', segment.shape, segment)
                    exit()
            plt.draw()


def nHandle(event):
    
    global coords
    global all_segment
    # coords.append([ix, iy])
    print('nHandle $$$$$$$$$$$$$$$$$$$$$$', len(all_segment[0]))
    global ax
    button_bounds = [[0.12, 1e-06], [0.27, 0.075001]]
    ix, iy = event.xdata, event.ydata
    print('nHandle called')
    
    # print('call nSegment @@@@@@@@@@', ix, iy, ix >=
        #   button_bounds[0], ix <= button_bounds[1], iy <= button_bounds[3], iy >= button_bounds[2])
    position_of_axis = event.inaxes.get_position().extents
    
    # ipdb.set_trace()
    if (ix is not None and iy is not None) and ((button_bounds[0][0] == position_of_axis[0]) and (button_bounds[1][0] == position_of_axis[2] and button_bounds[1][1]== position_of_axis[3])):
        # ipdb.set_trace()
        print('going in nHandle called')
        # print('call nSegment @@@@@@@@@@', ix, iy)
        all_segment.append(np.array(coords))
        coords = []


def find_closest_pt(x,y):
    global all_segment
    min_seg_index = None
    min_pt_index = None
    min_dist = None
    min_x , min_y = None, None
    for seg_index, segment in enumerate(all_segment):
        for pt_index, pt in enumerate(segment):
            dist = np.linalg.norm([pt[0]-x, pt[1]-y])
            if min_dist is None or dist < min_dist:
                min_pt_index = pt_index
                min_seg_index = seg_index
                min_x = pt[0]
                min_y = pt[1]
                min_dist = dist
    print()
    global_pt_index = min_pt_index
    global_seg_index = min_seg_index

    return global_seg_index, global_pt_index, min_x, min_y, min_dist 


# def add_points(event):
#     print('##### add points called')
#     global coords
#     global all_segment
#     coords.append([ix, iy])
#     all_segment[-1] = np.array(coords)
#     onclick(event)


def nFrame(img):
    print('generating new frame')
    return img


def ActivateMove(event):
    
    global current_segment
    global coords
    global flag_move
    print('ActivateMove $$$$$$$$$$$$$$$$$$$$$$', len(all_segment[0]))

    button_bounds = [[0.52, 0.000001], [6.7000e-01, 7.5001e-02]]
    ix, iy = event.xdata, event.ydata
    
    position_of_axis = event.inaxes.get_position().extents
    
    if (ix is not None and iy is not None) and ((button_bounds[0][0] == position_of_axis[0]) and (button_bounds[1][0] == position_of_axis[2] and button_bounds[1][1] == position_of_axis[3])):
        print('Move the Handles @@@@@@@', position_of_axis)
        
        flag_move = True
        sleep(1)
    
    print(flag_move)


def findSetClosestOnClick(event):
        global flag_move
        print(flag_move)
        if flag_move:
            print('move activate  #####################')
            ix, iy = event.xdata, event.ydata
            global_seg_index, global_pt_index, min_x, min_y, min_dist = find_closest_pt(ix, iy)
            global g_seg_index
            global g_pt_index
            g_seg_index = global_seg_index
            g_pt_index = global_pt_index
            print('length #####', len(all_segment[g_seg_index]))
            print('closest indices.....', min_x, min_y, min_dist, g_seg_index, g_pt_index)
            print(g_seg_index)
            print(g_pt_index)


def drag_update(event):
    global flag_move
    global g_seg_index
    global g_pt_index
    global drag_status
    global all_segment
    global ax
    global coords
    
    if flag_move and g_seg_index is not None and g_pt_index is not None :
        print('######## drag update')
        drag_status = True
        ix,iy = event.xdata, event.ydata
        print(all_segment[g_seg_index][g_pt_index], len(all_segment[g_seg_index]))
        all_segment[g_seg_index][g_pt_index][0] = ix
        all_segment[g_seg_index][g_pt_index][1] = iy
        if g_seg_index == len(all_segment)-1:
            coords = all_segment[g_seg_index].tolist()
    
        
        for segment in all_segment:
            try:
                ax.clear()
                ax.set_xlim(0, 300)
                ax.set_ylim(0, 300)
                ax.imshow(img, extent=(0, 250, 50, 300))
                ax.plot(segment[:,0], segment[:,1], markersize=10)
                ax.scatter(segment[:,0], segment[:,1] )
                print(np.array(all_segment).shape)
                plt.axis('off')
                plt.draw()
                
            except IndexError as e:
                print('error', segment.shape, segment)
                exit()
        # ax.clear()
        # ax.imshow(img, extent=(0, 250, 50, 300))
        
        
def deactivate_move(event):
    global drag_status
    global flag_move 
    global g_seg_index 
    global g_pt_index
    
    if drag_status:
        flag_move = False
        drag_status = False
        g_seg_index = None
        g_pt_index = None
        print('######## resetting')
        sleep(0.2)
        

def Clear(event):
    print('Clear all Handles')
    global all_segment
    global coords
    button_bounds = [[7.2000e-01, 1.0000e-06], [8.7000e-01, 7.5001e-02]]
    position_of_axis = event.inaxes.get_position().extents
    print(position_of_axis)
    if (ix is not None and iy is not None) and ((button_bounds[0][0] == position_of_axis[0]) and (button_bounds[1][0] == position_of_axis[2] and button_bounds[1][1] == position_of_axis[3])):
        coords = []
        all_segment = []
        ax.clear()
        ax.set_xlim(0, 300)
        ax.set_ylim(0, 300)
        ax.imshow(img, extent=(0, 250, 50, 300))
        plt.axis('off')
        plt.draw()


cid = fig.canvas.mpl_connect('button_press_event', nFrame)
cid = fig.canvas.mpl_connect('button_press_event', findSetClosestOnClick)
cid = fig.canvas.mpl_connect('motion_notify_event', drag_update)
cid = fig.canvas.mpl_connect('button_release_event', deactivate_move)
  

def PyMaurice(event):
    Maurice.initShape("maurice.png")
    


cid = fig.canvas.mpl_connect('button_press_event', ActivateMove)
cid = fig.canvas.mpl_connect('button_press_event', Clear)
cid = fig.canvas.mpl_connect('button_press_event', nHandle)
cid = fig.canvas.mpl_connect('button_press_event', onclick)


Button1 = plt.axes([0.12, 0.000001, 0.15, 0.075])
bnext = Button(Button1, 'nHandle',color="lightgreen")
bnext.on_clicked(nHandle)

Button2 = plt.axes([0.32, 0.000001, 0.15, 0.075])
bnext = Button(Button2, 'Save',color="lightpink")
bnext.on_clicked(Save)

Button3 = plt.axes([0.52, 0.000001, 0.15, 0.075])
bnext = Button(Button3, 'Move',color="grey")
bnext.on_clicked(ActivateMove)

Button4 = plt.axes([0.72, 0.000001, 0.15, 0.075])
bnext = Button(Button4, 'Clear',color="skyblue")
# bnext.on_clicked(Clear)


# Create fuction to be called when slider value is changed
def update(val):
    f = frame.val

# Call update function when slider value is changed
frame.on_changed(update)

# display graph
plt.show()
plt.draw()
