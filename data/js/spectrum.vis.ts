/// <reference path="visualization.d.ts" />

// Spectrum DukVis visualization

function update(type: UpdateType) {
    if (type === UpdateType.INIT)
        return "initialization";
    if (type === UpdateType.RESIZE)
        return "resize update";
}

function render(/* Take arguments or more globals? */) {
    let g = screen.graphics;
    let oldX = 0;
    let oldY = screen.height / 2.0;

    g.moveTo(oldX, oldY);

    fft.samples.forEach((sample: number, index: number) => {
        let x = (index / fft.samples.length) * screen.width;
        let y = (screen.height / 2.0) + sample;

        g.lineTo(x, y);
    });

    g.lineTo(screen.width, screen.height / 2);
}

module.name     = "Spectrum";
module.version  = "0.0.1";
module.update   = update;
module.render   = render;
