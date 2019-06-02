APPLICATION=MakeHL2Run6ePlots.py
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

python $APPLICATION --ratio --inclusive --momentum --nominal
python $APPLICATION --ratio --inclusive --momentum --weighted

python $APPLICATION --ratio --oneTrack --momentum --nominal
python $APPLICATION --ratio --oneTrack --momentum --weighted

python $APPLICATION --ratio --NTracks --momentum --nominal
python $APPLICATION --ratio --NTracks --momentum --weighted

python $APPLICATION -i -p -q -E -n
python $APPLICATION -o -p -q -E -n
python $APPLICATION -N -p -q -E -n

python $APPLICATION -i -p -q -E -w
python $APPLICATION -o -p -q -E -w
python $APPLICATION -N -p -q -E -w

python $APPLICATION -N -E -n
python $APPLICATION -N -E -w

python $APPLICATION -i -E -n
python $APPLICATION -o -E -n

python $APPLICATION -i -E -w
python $APPLICATION -o -E -w

# python $APPLICATION --ratio --inclusive --costheta --nominal
# python $APPLICATION --ratio --inclusive --costheta --weighted
# python $APPLICATION --ratio --oneTrack --costheta --nominal
# python $APPLICATION --ratio --oneTrack --costheta --weighted
# python $APPLICATION --ratio --NTracks --costheta --nominal
# python $APPLICATION --ratio --NTracks --costheta --weighted
