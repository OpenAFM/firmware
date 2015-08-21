This project is a library to control the hardware of the AFM (atomic force microscope) developed
during the lego2nano summer programme 2015. 

openafm.com

DACController - TLC5620CN Digital to analog converter (increase/decrease voltage to move piezos).

SignalSampler - Read signals and calculate pixel value (calculate focus error).

Scanner       - Use controller and sampler to activate piezos and move stage to scan area.
              - double pass scanning (trace forward and retrace backwards).

RTx           - GUI COM (read and write)