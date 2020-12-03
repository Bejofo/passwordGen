const express = require('express')
const app = express()
const port = 3000
const fs = require('fs');
const bodyParser = require('body-parser');
const md5File = require('md5-file')

// const repl = require('repl');


app.use(bodyParser.urlencoded({ extended: true }));

var jobs = [];
var keySpaceCallBack = null;

function generateJobs(pattern, chunks = 1000) {
    keySpaceCallBack = (keyspace) => { // Javascript async is painful
        var chunkSize = Math.floor( keyspace / chunks);
        if(isNaN(chunkSize) || chunkSize <= 0){
            console.error("Something went terribly wrong")
        }
        const checkSum = md5File.sync('./public/target.txt');

        for (var i = 0; i < keyspace; i += chunkSize) {
            jobs.push({
                status: "1", 
                start: i,
                checksum: checkSum,
                limit: i + chunkSize,
                pattern: pattern
            })
        }
        jobs.reverse();
    }
    jobs.push({
       status:"2", 
       pattern:pattern 
    });
}

app.get('/keyspace', (req, res) => {
    keySpaceCallBack(+req.query.q); // cast to int
    // console.log(+req.query.q);
    res.send('ok')
})

app.use('/public', express.static('public')) // Everything in the public folder is acessable 

app.get('/getjobs', (req, res) => {
    console.log(jobs.length);
    if (jobs.length == 0) {
        res.send(`{"status":"0"}`);
    }
    res.send(JSON.stringify(jobs.pop()));
})

app.get('/', (req, res) => {
    res.send(`<pre>Debug Info:
Serving on port ${port}
cmd = ${JSON.stringify([...jobs.slice(jobs.length - 100)].reverse(), null, '\t')}</pre>
	`)
})

app.post('/', (req, res) => {
    var q = req.body.q;
    console.log("File written");
    fs.writeFileSync(`${new Date().getTime()}.txt`, q)
    res.send('OK');
})

app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`)
})

var readline = require('readline');
var log = console.log;

var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

//https://stackoverflow.com/questions/24464404/how-to-readline-infinitely-in-node-js
// *** You can comment the following block safely if it is causing problems, its not needed. **
var recursiveAsyncReadLine = function () { 
    rl.question('> ', function (answer) {
        if (answer == 'exit') //we need some base case, for recursion
            process.exit(0);
        try {
            console.log(eval(answer));
        } catch (error) {
            console.error(error);
        }
        recursiveAsyncReadLine(); //Calling this function again to ask new question
    });
};

generateJobs("?a".repeat(8), 1e6);
recursiveAsyncReadLine(); 
