import json
import time
import os
import requests
import hashlib

#master_url = "http://localhost:3000"
target_file = "target.txt"
master_url= "http://10.6.22.72:3000" # change this


def run_job(cmd):
    le_cmd = f"hashcat -m 0 -a 3 {target_file} {cmd['pattern']} -o out.txt -s {cmd['start']} -l {cmd['limit']} --potfile-disable"
    print(le_cmd)
    # make sure they are the same file
    if (not os.path.exists("target.txt")) or md5("target.txt") != cmd["checksum"]:
        print("target.txt wasn't found or the hash didn't match. Downloading now...")
        r = requests.get(f"{master_url}/public/target.txt")
        open('target.txt', 'wb').write(r.content)
    os.system(le_cmd)
    try:
        output = open('out.txt', 'r').read()
        myobj = {'q': output}
        requests.post(master_url, data=myobj)
        os.system("rm out.txt")
    except:
    	print("Error reading out.txt")


def get_keyspace(cmd):
    ks = os.popen(f"hashcat -a 3 {cmd['pattern']} --keyspace").read()
    print(f"Keyspace is {ks}")
    requests.get(f"{master_url}/keyspace?q={ks}")


# https://stackoverflow.com/questions/3431825/generating-an-md5-checksum-of-a-file
def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()


while True:
    time.sleep(1)
    try:
        r = requests.get(f"{master_url}/getjobs",timeout=1)
    except:
       print("Master server not found. Retrying...")
       continue
    data = json.loads(r.text)
    print(data)
    if data["status"] == "0": # don't do anything
        continue
    else:
        if data["status"] == "2": #return keyspace
            get_keyspace(data)
        else:
            run_job(data)
