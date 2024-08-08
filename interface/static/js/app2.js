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
socket.on('connect1', function() {
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
    c=0
    if (Array.isArray(data.channel6)) {
        // console.log('Processing channel6 data:', data.channel6);
        data.channel6.forEach(value => {
            // console.log('Appending value to channel6:', value);
            timeSeries6.append(timestamp+c, value);
            c+=1
        });
    } else {
        console.error('Invalid data format for channel6');
    }
});
