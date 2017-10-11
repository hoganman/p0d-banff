#!/bin/python
import os

""" MC 6B """

# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/MC_Productions/GRID/mcp6_Spin_B/neut/run2-air/anal/Run2_Air.list -p /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run2-air -o Run2_Air -M 60 -g NEUT -n 100')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/MC_Productions/GRID/mcp6_Spin_B/neut/run2-water/anal/Run2_Water.list -p /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run2-water -o Run2_Water -M 60 -g NEUT -n 100')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/MC_Productions/GRID/mcp6_Spin_B/neut/run3-air/anal/Run3_Air.list -p /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run3-air -o Run3_Air -M 60 -g NEUT -n 100')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/MC_Productions/GRID/mcp6_Spin_B/neut/run4-water/anal/Run4_Water.list -p /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run4-water -o Run4_Water -M 60 -g NEUT -n 100')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/MC_Productions/GRID/mcp6_Spin_B/neut/run4-air/anal/Run4_Air.list -p /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run4-air -o Run4_Air -M 60 -g NEUT -n 100')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/MC_Productions/GRID/mcp6_Spin_B/anti-neut/run5-water/anal/Run5_Water.list -p /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/anti-neut/run5-water -o Run5_Water -M 60 -g NEUT -n 100')


os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run2-air -p /physics/home/mhogan/splines/highlandIOv2r21/mcp6_Spin_B/neut/run2-air -o Run2_Air_spline -M 60 -g MC')
os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run2-water -p /physics/home/mhogan/splines/highlandIOv2r21/mcp6_Spin_B/neut/run2-water -o Run2_Water_spline -M 60 -g MC')
os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run3-air -p /physics/home/mhogan/splines/highlandIOv2r21/mcp6_Spin_B/neut/run3-air -o Run3_Air_spline -M 60 -g MC')
os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run4-water -p /physics/home/mhogan/splines/highlandIOv2r21/mcp6_Spin_B/neut/run4-water -o Run4_Water_spline -M 60 -g MC')
os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/neut/run4-air -p /physics/home/mhogan/splines/highlandIOv2r21/mcp6_Spin_B/neut/run4-air -o Run4_Air_spline -M 60 -g MC')
os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/mcp6_Spin_B/anti-neut/run5-water -p /physics/home/mhogan/splines/highlandIOv2r21/mcp6_Spin_B/anti-neut/run5-water -o Run5_Water_spline -M 60 -g MC')

""" Data 6M """

# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/Data_Productions/GRID/rdp6_Spin_M/UK/run2/anal/Run2_Air.list -p /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run2-air -o Run2_Air_data -M 60 -g data -n 10')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/Data_Productions/GRID/rdp6_Spin_M/UK/run2/anal/Run2_Water.list -p /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run2-water -o Run2_Water_data -M 60 -g data -n 10')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/Data_Productions/GRID/rdp6_Spin_M/UK/run3/anal/Run3_Air.list -p /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run3-air -o Run3_Air_data -M 60 -g data -n 10')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/Data_Productions/GRID/rdp6_Spin_M/UK/run4/anal/Run4_Water.list -p /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run4-water -o Run4_Water_data -M 60 -g data -n 10')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/Data_Productions/GRID/rdp6_Spin_M/UK/run4/anal/Run4_Air.list -p /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run4-air -o Run4_Air_data -M 60 -g data -n 10')
# os.system('nohup python qsubmitter.py -q "physics.q" -m 1024 -L /physics/data/Data_Productions/GRID/rdp6_Spin_M/UK/run5/anal/Run5_Water_RHC.list -p /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run5-water -o Run5_Water_data -M 60 -g data -n 10')


# os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run2-air -p /physics/home/mhogan/splines/highlandIOv2r21/rdp6_Spin_M/run2-air -o Run2_Air_data_spline -M 60 -g data')
# os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run2-water -p /physics/home/mhogan/splines/highlandIOv2r21/rdp6_Spin_M/run2-water -o Run2_Water_data_spline -M 60 -g data')
# os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run3-air -p /physics/home/mhogan/splines/highlandIOv2r21/rdp6_Spin_M/run3-air -o Run3_Air_data_spline -M 60 -g data')
# os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run4-water -p /physics/home/mhogan/splines/highlandIOv2r21/rdp6_Spin_M/run4-water -o Run4_Water_data_spline -M 60 -g data')
# os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run4-air -p /physics/home/mhogan/splines/highlandIOv2r21/rdp6_Spin_M/run4-air -o Run4_Air_data_spline -M 60 -g data')
# os.system('nohup python qsubmitter_splines.py -q "physics.q" -m 1024 -L /physics/data/flattree/highlandIOv2r21/rdp6_Spin_M/run5-water -p /physics/home/mhogan/splines/highlandIOv2r21/rdp6_Spin_M/run5-water -o Run5_Water_data_spline -M 60 -g data')
