

import bpy
import struct
import re
import os
import mathutils
import bmesh
import builtins as __builtin__

def console_print(*args, **kwargs):
    for a in bpy.context.screen.areas:
        if a.type == 'CONSOLE':
            c = {}
            c['area'] = a
            c['space_data'] = a.spaces.active
            c['region'] = a.regions[-1]
            c['window'] = bpy.context.window
            c['screen'] = bpy.context.screen
            s = " ".join([str(arg) for arg in args])
            for line in s.split("\n"):
                bpy.ops.console.scrollback_append(c, text=line)

def print(*args, **kwargs):
    """Console print() function."""
    console_print(*args, **kwargs) # to py consoles
    __builtin__.print(*args, **kwargs) # to system console

eps = .001

def bake_rotation(obj):
    return

def isclose(a,b):
    xa,ya,za = a
    xb,yb,zb = b
    return abs(xa-xb)<eps and abs(ya-yb)<eps and abs(za-zb)<eps

def iscloseInter(A,B):
    if len(A)!=len(B): return None
    out = []
    for a in A:
        for b,i in B:
            if isclose(a,b):
                out.append(i)
                break
        else: return None
    return out


bpy.ops.object.mode_set(mode = 'OBJECT')
bpy.ops.object.select_all()
#obj = bpy.context.active_object
bpy.ops.object.mode_set(mode = 'EDIT') 
bpy.ops.mesh.select_mode(type="VERT")
bpy.ops.mesh.select_all(action = 'DESELECT')
bpy.ops.object.mode_set(mode = 'OBJECT')


print(os.getcwd())

print("spiejpfisje")
number = struct.Struct('l')
integer = struct.Struct('i')
smallnumber = struct.Struct('I')
vertex = struct.Struct('ffffff')
triangle = struct.Struct('lll')
interfaces = []
def getInterfaceIndex(two):
    for i in range(len(interfaces)):
        ss = iscloseInter(interfaces[i],two)
        if ss!=None: return (i,ss)
    else:
        interfaces.append([o[0] for o in two])
        return (len(interfaces)-1,[o[1] for o in two])
def saveII(f,ii):
    i,ss = ii
    f.write(number.pack(i))
    f.write(number.pack(len(ss)))
    for s in ss: f.write(number.pack(s))

def saveInterfaceIndex(f,two):
    saveII(f,getInterfaceIndex(two))


with open(bpy.path.abspath("//tileset"),"wb") as f:
    debug = len([i for i in bpy.context.scene.objects.keys() if re.search(r'[0-9]+x[0-9]+x[0-9]+$',i)])
    print(debug)
    f.write(number.pack(debug))
    for name,obj in bpy.context.scene.objects.items():
        if not re.search(r'[0-9]+x[0-9]+x[0-9]+$',name): continue
        bake_rotation(obj)
        size = [int(i) for i in re.search(r'[0-9]+x[0-9]+x[0-9]+$',name).group().split('x')]
        xs,ys,zs = size
        f.write(smallnumber.pack(len(name)))
        f.write(struct.Struct(str(len(name))+'s').pack(bytes(name,'utf-8')))
        mesh = obj.data
        mesh.calc_loop_triangles()
        f.write(number.pack(len(mesh.vertices)))
        planes = (
            (
                [[[] for z in range(zs)] for y in range(ys)],
                [[[] for z in range(zs)] for y in range(ys)]
            ),(
                [[[] for x in range(xs)] for z in range(zs)],
                [[[] for x in range(xs)] for z in range(zs)]
            ),(
                [[[] for y in range(ys)] for x in range(xs)],
                [[[] for y in range(ys)] for x in range(xs)]
            )
        )
        for v1 in range(len(mesh.vertices)):
            for v2 in range(v1):
                if isclose(mesh.vertices[v1].co,mesh.vertices[v2].co):
                    mesh.vertices[v1].select = True
                    mesh.vertices[v2].select = True
                    print(v1,v2)
                    bpy.ops.object.mode_set(mode = 'EDIT')
                    assert False
        for ind in range(len(mesh.vertices)):
            vert = mesh.vertices[ind]
            co    = tuple((i+1)/2.0 for i in vert.co)
            close = tuple(abs(i-round(i))<eps for i in co)
            ve    = tuple(int(round(i)) for i in co)
            edge  = tuple(ve[i]==0 or ve[i]==size[i] for i in range(3))
            for i in range(3):
                for extreme in range(2):
                    if close[i] and (0,size[i])[extreme]==ve[i]:
                        if (close[i-1] and not edge[i-1]) or (close[i-2] and not edge[i-2]):
                            print(co,close,ve,edge)
                            vert.select = True
                            bpy.ops.object.mode_set(mode = 'EDIT')
                            assert False
                        v = [int(j) for j in co]
                        if v[i-1]<0 or v[i-2]<0 or v[i-2]>=len(planes[i][extreme]) or v[i-1]>=len(planes[i][extreme][v[i-2]]): continue
                        conc = planes[i][extreme][v[i-2]][v[i-1]]
                        v[i] = ve[i]-.5
                        conc.append((tuple(co[j]-.5-v[j] for j in range(3)),ind))
            f.write(vertex.pack(*vert.co,*vert.normal))
        f.write(number.pack(len(mesh.loop_triangles)))
        for tri in mesh.loop_triangles:
            f.write(triangle.pack(*tri.vertices))
        for dim in range(3):
            for extreme in range(2):
                for x in range(size[dim-2]):
                    for y in range(size[dim-1]):
                        if len(planes[dim][x][y]):
                            v = [x,y][::-1 if dim==1 else 1]
                            v.insert(dim,size[dim] if extreme else 0)
                            for s in v: f.write(integer.pack(s))
                            saveInterfaceIndex(f,planes[dim][extreme][x][y])
    tosave = []
    j = 0
    while j<len(interfaces):
        d = interfaces[j]
        rot90  = [(( y,-x,z),ind) for ind,(x,y,z) in enumerate(d)]
        rot180 = [((-x,-y,z),ind) for ind,(x,y,z) in enumerate(d)]
        rot270 = [((-y, x,z),ind) for ind,(x,y,z) in enumerate(d)]
        tosave.append(getInterfaceIndex(rot90))
        tosave.append(getInterfaceIndex(rot180))
        tosave.append(getInterfaceIndex(rot270))
        j+=1
    f.write(number.pack(len(interfaces)))
    for save in tosave: saveII(f,save)



  
bpy.ops.object.mode_set(mode = 'EDIT')









        



