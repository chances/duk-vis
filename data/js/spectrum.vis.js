function update(type) {
    if (type === 0)
        return "initialization?";
    if (type === 1)
        return "resize update";
}
function render() {
    var g = screen.graphics;
    var oldX = 0;
    var oldY = screen.height / 2.0;
    g.moveTo(oldX, oldY);
    fft.samples.forEach(function (sample, index) {
        var x = (index / fft.samples.length) * screen.width;
        var y = (screen.height / 2.0) + sample;
        g.lineTo(x, y);
    });
    g.lineTo(screen.width, screen.height / 2);
}
module.name = "Spectrum";
module.version = "0.0.1";
module.update = update;
module.render = render;
