// The UI code lives in main/src/ui so main.ino can use it too. Arduino only
// compiles .cpp files inside a sketch's own folder, so this test sketch pulls
// them in here. Add a line when you add a component or screen file.
// (They all end up in this one file, so file-level `static` names must be
// unique across files -- e.g. heatingRing vs baselineRing.)

#include "../src/ui/Gfx.cpp"
#include "../src/ui/Button.cpp"

#include "../src/ui/components/ProgressBar.cpp"
#include "../src/ui/components/Ring.cpp"
#include "../src/ui/components/MenuList.cpp"
#include "../src/ui/components/ValueFormat.cpp"
#include "../src/ui/components/MetricRow.cpp"
#include "../src/ui/components/RangeLegend.cpp"

#include "../src/ui/screens/TestPattern.cpp"
#include "../src/ui/screens/Splash.cpp"
#include "../src/ui/screens/Menu.cpp"
#include "../src/ui/screens/Heating.cpp"
#include "../src/ui/screens/Baselining.cpp"
#include "../src/ui/screens/Sensing.cpp"
#include "../src/ui/screens/Metrics.cpp"
#include "../src/ui/screens/Saving.cpp"
#include "../src/ui/screens/SaveFailed.cpp"
#include "../src/ui/screens/Done.cpp"
#include "../src/ui/screens/Memory.cpp"
