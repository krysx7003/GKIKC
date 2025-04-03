import sys

from glfw.GLFW import *

from OpenGL.GL import *
from OpenGL.GLU import *

def startup():
    glClearColor(0.5,0.5,0.5,1.0)

def shutdown():
    pass

def render(time,size):
    glClear(GL_COLOR_BUFFER_BIT)
    sierpinski(size,0,6,-1,-1)


def sierpinski(size,depth,desired_depth,start_x,start_y):
    edge=size/3
    y=start_y
    i=0
    while  i<3:   
        j=0
        x = start_x
        while j<3:
            if not (j==1 and i==1):
                if depth==desired_depth:
                    draw_square(x,y,edge) 
                    glFlush()
                else:
                    sierpinski(edge,depth+1,desired_depth,x,y)
            x +=edge
            j += 1
        y +=edge
        i += 1

    

def draw_square(x,y,a):

    glColor3f(0.0,0.0,0.0)
    glBegin(GL_TRIANGLES)
    glVertex2f(x,y)
    glVertex2f(x,y+a)
    glVertex2f(x+a,y)
    glEnd()

    glBegin(GL_TRIANGLES)
    glVertex2f(x+a,y+a)
    glVertex2f(x+a,y)
    glVertex2f(x,y+a)
    glEnd()



def main():
    if not glfwInit():
        sys.exit(-1)
    
    window = glfwCreateWindow(729,729,__file__,None,None)
    if not window:
        glfwTerminate()
        sys.exit(-2)

    glfwMakeContextCurrent(window)
    glfwSwapInterval(1)

    startup()

    while not glfwWindowShouldClose(window):
        render(glfwGetTime(),2)
        glfwSwapBuffers(window)
        glfwWaitEvents()

    shutdown()
    glfwTerminate()

if __name__ == '__main__':
    main() 