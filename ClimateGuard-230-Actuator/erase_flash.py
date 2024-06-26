Import("env")
env.AddPreAction("upload", "erase_flash")

def erase_flash(source, target, env):
    env.Execute("esptool.py --port $UPLOAD_PORT erase_flash")

env.Replace(UPLOADCMD=erase_flash)
