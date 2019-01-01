#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_physfs.h>
#include <physfs.h>

#include "Gwen/Gwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"
#include "Gwen/Input/Allegro.h"
#include "Gwen/Renderers/Allegro.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/Label.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/NumericUpDown.h"
#include "Gwen/Controls/ComboBox.h"
#include "Gwen/Controls/GroupBox.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/ImagePanel.h"

#include "ini.h"

#include <iostream>
#include <string>
#include <list>
#include <random>
#include <sys/stat.h>
#include <math.h>

using namespace std;
using namespace Gwen;