import os

def getsub(d, a):
    for o in os.listdir(d):
        subpath = os.path.join(d, o)
        if os.path.isdir(subpath):
            getsub(subpath, a)
        else:
            a.append(o)

def delete(d, a):
    for o in os.listdir(d):
            subpath = os.path.join(d, o)
            if os.path.isdir(subpath):
                delete(subpath, a)
            else:
                if (o in a):
                    path = os.path.join(d, o)
                    os.remove(path)
                    # print(path + "removed!")


a = []
getsub("/Users/Wei/Downloads/helloworld", a)
print(a)
delete("/Volumes/Seagate Expansion Drive/init", a)
