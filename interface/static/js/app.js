var smoothie1 = new SmoothieChart({millisPerPixel:10,grid:{strokeStyle:'rgba(119,119,119,0.70)'},tooltipLine:{strokeStyle:'#bbbbbb'}});
var smoothie2 = new SmoothieChart({millisPerPixel:10,grid:{strokeStyle:'rgba(119,119,119,0.70)'},tooltipLine:{strokeStyle:'#bbbbbb'}});
var smoothie3 = new SmoothieChart({millisPerPixel:10,grid:{strokeStyle:'rgba(119,119,119,0.70)'},tooltipLine:{strokeStyle:'#bbbbbb'}});
var smoothie4 = new SmoothieChart({millisPerPixel:10,grid:{strokeStyle:'rgba(119,119,119,0.70)'},tooltipLine:{strokeStyle:'#bbbbbb'}});
var smoothie5 = new SmoothieChart({millisPerPixel:10,grid:{strokeStyle:'rgba(119,119,119,0.70)'},tooltipLine:{strokeStyle:'#bbbbbb'}});
smoothie1.streamTo(document.getElementById('chart1'));
smoothie2.streamTo(document.getElementById('chart2'));
smoothie3.streamTo(document.getElementById('chart3'));
smoothie4.streamTo(document.getElementById('chart4'));
smoothie5.streamTo(document.getElementById('chart5'));
var socket = io.connect('http://' + document.domain + ':' + location.port)
var timeSeries1 = new TimeSeries();
var timeSeries2 = new TimeSeries();
var timeSeries3 = new TimeSeries();
var timeSeries4 = new TimeSeries();
var timeSeries5 = new TimeSeries()
smoothie1.addTimeSeries(timeSeries1);
smoothie2.addTimeSeries(timeSeries2);
smoothie3.addTimeSeries(timeSeries3);
smoothie4.addTimeSeries(timeSeries4);
smoothie5.addTimeSeries(timeSeries5)
socket.on('connect', function() {
    console.log('Connected to server');
})
socket.on('disconnect', function() {
    console.log('Disconnected from server');
})


socket.on('data', function(data) {
    // console.log('Received data:', data)
    var timestamp = new Date().getTime();
    // console.log('Current timestamp:', timestamp);
    c=0;
    // Ensure data is an array of values for each channel
    if (Array.isArray(data.channel1)) {
        // console.log('Processing channel1 data:', data.channel1);
        data.channel1.forEach(value => {
            // console.log('Appending value to channel1:', value);
            timeSeries1.append(timestamp+c, value);
            c+=1
        });
    } else {
        console.error('Invalid data format for channel1');
    }
    c=0
    if (Array.isArray(data.channel2)) {
        // console.log('Processing channel2 data:', data.channel2);
        data.channel2.forEach(value => {
            // console.log('Appending value to channel2:', value);
            timeSeries2.append(timestamp+c, value);
            c+=1
        });
    } else {
        // console.error('Invalid data format for channel2');
    }
    c=0
    if (Array.isArray(data.channel3)) {
        // console.log('Processing channel3 data:', data.channel3);
        data.channel3.forEach(value => {
            // console.log('Appending value to channel3:', value);
            timeSeries3.append(timestamp+c, value);
            c+=1
        });
    } else {
        console.error('Invalid data format for channel3');
    }
    c=0
    if (Array.isArray(data.channel4)) {
        // console.log('Processing channel4 data:', data.channel4);
        data.channel4.forEach(value => {
            // console.log('Appending value to channel4:', value);
            timeSeries4.append(timestamp+c, value);
            c+=1
        });
    } else {
        console.error('Invalid data format for channel4');
    }
    c=0
    if (Array.isArray(data.channel5)) {
        // console.log('Processing channel5 data:', data.channel5);
        data.channel5.forEach(value => {
            // console.log('Appending value to channel5:', value);
            timeSeries5.append(timestamp+c, value);
            c+=1
        });
    } else {
        console.error('Invalid data format for channel5');
    }
});

let gifIndex = 0;
let buttonIndex = 0;
const gifElement = document.getElementById('gif');
const headingElement = document.getElementById('heading');
const buttons = document.querySelectorAll('.buttons button');

function changeGif() {
    gifIndex = gifIndex + 1;
    gifIndex1 = gifIndex % gifs.length;
    gifElement.src = gifs[gifIndex1].src;
    headingElement.textContent = gifs[gifIndex1].heading;
    updateIntervals();
}

function blinkButtons() {
    buttons.forEach(button => button.classList.remove('blink'));
    buttonIndex = (buttonIndex + 1) % buttons.length;
    buttons[buttonIndex].classList.add('blink');
}

let gifInterval = setInterval(changeGif, 24000);
let buttonInterval = setInterval(blinkButtons, 4000);

function updateIntervals() {
    if (gifIndex == 0) {
        clearInterval(gifInterval);
//        clearInterval(buttonInterval);
    } else if (gifIndex < 5) {
        clearInterval(gifInterval);
//        clearInterval(buttonInterval);
        gifInterval = setInterval(changeGif, 24000);
//        buttonInterval = setInterval(blinkButtons, 1000);
    } else if (gifIndex < 10) {
        console.log(gifIndex,buttonIndex)
        clearInterval(gifInterval);
        //clearInterval(buttonInterval);
        gifInterval = setInterval(changeGif, 12000);
        //buttonInterval = setInterval(blinkButtons, 2000);
    } else {
        clearInterval(gifInterval);
        clearInterval(buttonInterval);
        socket.emit('stop_recording');
        alert('Thank you for recording');
    }
    if (gifIndex==5){
    clearInterval(buttonInterval);
    buttonInterval = setInterval(blinkButtons, 2000);}
}


document.getElementById('stopButton').addEventListener('click', () => {
    socket.emit('stop_recording');
});