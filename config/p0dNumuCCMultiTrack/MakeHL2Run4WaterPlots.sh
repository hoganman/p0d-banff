# Options:
#   -r, --ratio      Run make a ratio plot only
#   -i, --inclusive  Run CCInc only
#   -o, --oneTrack   Run CC 1-track only
#   -N, --NTracks    Run CC n-track only
#   -p, --momentum   Analyze momentum
#   -q, --costheta   Analyze costheta
#   -E, --Enu        Analyze Enu
#   -n, --nominal    Turn off all weights
#   -w, --weighted   Use all weights

# python MakeHL2Run4WaterPlots.py --ratio --inclusive --momentum --nominal
# python MakeHL2Run4WaterPlots.py --ratio --inclusive --costheta --nominal
# python MakeHL2Run4WaterPlots.py --ratio --inclusive --momentum --weighted
# python MakeHL2Run4WaterPlots.py --ratio --inclusive --costheta --weighted
# 
# python MakeHL2Run4WaterPlots.py --ratio --oneTrack --momentum --nominal
# python MakeHL2Run4WaterPlots.py --ratio --oneTrack --costheta --nominal
# python MakeHL2Run4WaterPlots.py --ratio --oneTrack --momentum --weighted
# python MakeHL2Run4WaterPlots.py --ratio --oneTrack --costheta --weighted
# 
# python MakeHL2Run4WaterPlots.py --ratio --NTracks --momentum --nominal
# python MakeHL2Run4WaterPlots.py --ratio --NTracks --costheta --nominal
# python MakeHL2Run4WaterPlots.py --ratio --NTracks --momentum --weighted
# python MakeHL2Run4WaterPlots.py --ratio --NTracks --costheta --weighted

# python MakeHL2Run4WaterPlots.py -i -p -q -E -n
# python MakeHL2Run4WaterPlots.py -o -p -q -E -n
# python MakeHL2Run4WaterPlots.py -N -p -q -E -n

# python MakeHL2Run4WaterPlots.py -i -p -q -E -w
# python MakeHL2Run4WaterPlots.py -o -p -q -E -w
# python MakeHL2Run4WaterPlots.py -N -p -q -E -w

# python MakeHL2Run4WaterPlots.py -N -E -n
# python MakeHL2Run4WaterPlots.py -N -E -w

# python MakeHL2Run4WaterPlots.py -i -E -n
# python MakeHL2Run4WaterPlots.py -o -E -n
# 
# python MakeHL2Run4WaterPlots.py -i -E -w
# python MakeHL2Run4WaterPlots.py -o -E -w

