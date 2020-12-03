# How to use

## master.js

If NodeJs isn't installed, installing it would a good first step.
On linux it would look something like this.
```
sudo apt install nodejs.
```
You will also need to install a bunch of dependencies. Running `npm install` in the correct directory should install them.

To start the server `nodejs master.js` should work

Then move the hashes you want to crack into `/static` and name it `target.txt`. By default there should be already one there. It should contain of md5 hashes seperated by newlines. I got the hashes from https://hashes.org/leaks.php and cleaned it up using cleanup.py.

Once started by default the server will start
queuing jobs to crack all 8 character passwords. If you want to clear all jobs and instead crack all 8 digits passwords that only use numbers then type in.
```
jobs = []
generateJobs("?d".repeat(8),100);
```

the `generateJobs()` method takes in a string, indicating the pattern to be cracked, and how many jobs it should be split up into. In this case, it will split the task into 100 different jobs.

If there are any workers on the network, then you should start to see files being created in the directory. Those are the cracked hashes.
## worker.py
First install hashcat
```
sudo apt install hashcat
```

Then change url inside to the address where master.js is running.

Then `python3 worker.py` and you should be good. 


## cleanup.py
All it does is remove the plaintext from files in the following format so that hashcat can properly parse them.
```
d8578edf8458ce06fbc5bb76a58c5ca4:qwerty
cc3a0280e4fc1415930899896574e118:password
03e3e94c83089b5b41cf74e8c472f25a:bob1984
```

becomes 

```
d8578edf8458ce06fbc5bb76a58c5ca4
cc3a0280e4fc1415930899896574e118
03e3e94c83089b5b41cf74e8c472f25a
```
