/// <reference path="./core.d.ts"/>

declare const enum UpdateType {
    INIT = 0,
    RESIZE = 1,
    AUDIO = 2,
    RENDER = 3
}

/**
 * A container for the visualization as well as its user defined metadata
 */
interface Module {
    /**
     * Name of the visualization
     */
    name: string;
    /**
     * Semantic version of the visualization
     */
    version: string;

    /**
     * Update the visualization
     *
     * Called by the host at least once before a frame is ready for rendering.
     */
    update: (type: UpdateType) => void;

    /**
     * Render the visualization
     *
     * Called by the host every frame when ready to draw the visualization.
     */
    render: () => void;
}

/**
 * A graphical interface to draw to the screen
 *
 * All numerical parameters are floored unless otherwise specified.
 * All screen coordinates are clamped to the bounds of the screen.
 */
interface Graphics {
    /**
     * Move the pen to a specified postion
     *
     * Note: this operation does not draw anything.
     */
    moveTo: (x: number, y: number) => void;
    /**
     * Draw a line from the pen's current postion to a specified postion
     */
    lineTo: (x: number, y: number) => void;

    // Arc reference
    // https://msdn.microsoft.com/en-us/library/hk087c6d(v=vs.110).aspx
}

/**
 * The current FFT audio data
 */
interface FFT {
    samples: number[];
}

/**
 * The screen being drawn to by the visualization
 */
interface Screen {
    width: number;
    height: number;

    /**
     * A graphical interface to draw to the screen
     *
     * All numerical parameters are floored unless otherwise specified.
     * All screen coordinates are clamped to the bounds of the screen.
     */
    graphics: Graphics;
}

declare var dukvis: {
    module: Module;
    fft: FFT;
    screen: Screen;
};

/**
 * A container for the visualization as well as its user defined metadata
 */
declare var module: Module;
/**
 * The current FFT audio data
 */
declare var fft: FFT;
/**
 * The screen being drawn to by the visualization
 */
declare var screen: Screen;
