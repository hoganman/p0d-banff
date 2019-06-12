DrawingTools draw = DrawingTools("/physics/home/mhogan/p0dNumuCCMultiTrack/mcp6_Spin_B/neut/run4-air/Run4_Air.root");
DataSample mc = DataSample("/physics/home/mhogan/p0dNumuCCMultiTrack/mcp6_Spin_B/neut/run4-air/Run4_Air.root");
DataSample data = DataSample("/physics/home/mhogan/p0dNumuCCMultiTrack/rdp6_Spin_M/run4-air/Run4_Air_data.root");
Experiment run4a = Experiment("run4a", "run4_water_out.cfg");

void LoadRun4WaterOut()
{

printf("DrawingTools draw = DrawingTools(\"/physics/home/mhogan/p0dNumuCCMultiTrack/mcp6_Spin_B/neut/run4-air/Run4_Air.root\");\n");
printf("DataSample mc = DataSample(\"/physics/home/mhogan/p0dNumuCCMultiTrack/mcp6_Spin_B/neut/run4-air/Run4_Air.root\");\n");
printf("DataSample data = DataSample(\"/physics/home/mhogan/p0dNumuCCMultiTrack/rdp6_Spin_M/run4-air/Run4_Air_data.root\");\n");
printf("Experiment run4a = Experiment(\"run4a\", \"run4_water_out.cfg\");\n");
printf("Loaded Run 4 Water Out\n");

}
