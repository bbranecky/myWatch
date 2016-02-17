#include <pebble.h>

#define TIME_HEIGHT       49

#define FONT_DAY          32//not used
#define DAY_HEIGHT        32 //no padding needed below
#define DAY_WIDTH         58 //added to shift into center

#define FONT_WEEK         30//not used
#define WEEK_TOP_PAD      1
#define WEEK_HEIGHT       30 //no padding needed below
#define WEEK_WIDTH        58

#define TEMP_HEIGHT       34 //no padding needed below
#define TEMP_WIDTH        56

#define TEMP_HL_HEIGHT    29 //no padding needed below
#define TEMP_HL_WIDTH     42

#define FONT_LOC          30//not used
#define LOC_BOT_PAD       1
#define LOC_HEIGHT        30 //no padding needed below

#define FONT_LOCTIME      30//not used
#define LOCTIME_WIDTH     58
#define LOCTIME_HEIGHT    22 

Window *window;
TextLayer *textLayerTime;
TextLayer *textLayerWeekDay;
TextLayer *textLayerDay;
TextLayer *textLayerTemp;
TextLayer *textLayerTempHi;
TextLayer *textLayerTempLo;
TextLayer *textLayerLocation;
TextLayer *textLayerLocTime;
static GBitmap *s_bitmap;
static GBitmap *sub_bitmap;
static BitmapLayer *s_bitmap_layer;

static GFont s_time_font;
static GFont s_weekDay_font;
static GFont s_day_font;
static GFont s_temp_font;
static GFont s_temp_hl_font;
static GFont s_location_font;
static GFont s_locTime_font;

static void update_time() {
//  char text[50];

  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  static char timeStr[10];
  static char weekDayStr[15];
  static char dayStr[5];

  //--------- update Time ---------
  // Write the current hours and minutes into a buffer
  // Display this time on the TextLayer
  //strftime(timeStr, sizeof(timeStr), clock_is_24h_style() ? "%k:%M" : "%l:%M", tick_time); //%l no leading zero
  strftime(timeStr, sizeof(timeStr), "%l:%M", tick_time); //%l no leading zero
  //snprintf(timeStr, sizeof(timeStr), "%2d:%02d",(int)tick_time->tm_hour,(int)tick_time->tm_min );
  text_layer_set_text(textLayerTime, timeStr);
  //strftime(timeStr, sizeof(timeStr), "%a, %b %e \n %l:%M ", tick_time); //mon day %a %b
	//APP_LOG(APP_LOG_LEVEL_DEBUG,"%s", timeStr);
  
  //------- update Day number -------
  strftime(weekDayStr, sizeof(weekDayStr), "%a", tick_time); //%A no leading zero
  text_layer_set_text(textLayerWeekDay, weekDayStr);

  //------ update Day of the Week string ------
  strftime(dayStr, sizeof(dayStr), "%e", tick_time); //%A no leading zero
  text_layer_set_text(textLayerDay, dayStr);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void handle_init(void) {

  // Create a window and text layer
	window = window_create();
  //144 × 168
  //----------------------------------  over,down,width,height ------
  textLayerTime = text_layer_create(GRect(0,0,144,TIME_HEIGHT)); //numbers only need to padding
  textLayerWeekDay = text_layer_create(GRect(144-DAY_WIDTH-WEEK_WIDTH, TIME_HEIGHT+WEEK_TOP_PAD, WEEK_WIDTH, WEEK_HEIGHT));
  textLayerDay = text_layer_create(GRect(144-DAY_WIDTH, TIME_HEIGHT, WEEK_WIDTH, DAY_HEIGHT));
  textLayerTemp = text_layer_create(GRect(45, TIME_HEIGHT+DAY_HEIGHT+5,TEMP_WIDTH, TEMP_HEIGHT));
  //added 1 to the hi/lo to overlap location slightly to give more room
  textLayerTempHi = text_layer_create(GRect(144-TEMP_HL_WIDTH-2, 168-LOC_HEIGHT-2*TEMP_HL_HEIGHT+1, TEMP_HL_WIDTH, TEMP_HL_HEIGHT));
  textLayerTempLo = text_layer_create(GRect(144-TEMP_HL_WIDTH-2, 168-LOC_HEIGHT-LOC_BOT_PAD-TEMP_HL_HEIGHT+1, TEMP_HL_WIDTH, TEMP_HL_HEIGHT));
  textLayerLocation = text_layer_create(GRect(0, 168-LOC_HEIGHT-LOC_BOT_PAD, 144, LOC_HEIGHT));
  //added 5 because time does not have hanging letters (like g or j)
  textLayerLocTime = text_layer_create(GRect(45, 168-LOC_HEIGHT-LOC_BOT_PAD-LOCTIME_HEIGHT+4, LOCTIME_WIDTH, LOCTIME_HEIGHT));
	
   //----time ---
  text_layer_set_font(textLayerTime, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));

  //--- week day: mon,tue... ---
  s_weekDay_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_30));
  text_layer_set_font(textLayerWeekDay, s_weekDay_font);  
  //text_layer_set_font(textLayerDate, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
 
  //----- Day #: 31 ------
  s_day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_32));
  text_layer_set_font(textLayerDay, s_day_font);  

  //----- Temperature ----
  s_temp_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_33));
  text_layer_set_font(textLayerTemp, s_temp_font);  
  text_layer_set_text(textLayerTemp, "-39");

  //----- Temperature Hi/Lo----
  s_temp_hl_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_28));
  text_layer_set_font(textLayerTempHi, s_temp_hl_font);  
  text_layer_set_font(textLayerTempLo, s_temp_hl_font);  
  text_layer_set_text(textLayerTempHi, "120");
  text_layer_set_text(textLayerTempLo, "-30");
 
  //----- Location ----
  s_location_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_26));
  text_layer_set_font(textLayerLocation, s_location_font);  
  text_layer_set_text(textLayerLocation, "OconOmowoc");
  
  //----- Location Update Time ----
  s_locTime_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_20));
  text_layer_set_font(textLayerLocTime, s_locTime_font);  
  text_layer_set_text(textLayerLocTime, "12:30");
  
  //------ set alignments ----
  text_layer_set_text_alignment(textLayerTime, GTextAlignmentCenter);
	text_layer_set_text_alignment(textLayerWeekDay, GTextAlignmentCenter);
	text_layer_set_text_alignment(textLayerDay, GTextAlignmentLeft);
	text_layer_set_text_alignment(textLayerTemp, GTextAlignmentCenter);
	text_layer_set_text_alignment(textLayerTempHi, GTextAlignmentRight);
	text_layer_set_text_alignment(textLayerTempLo, GTextAlignmentRight);
	text_layer_set_text_alignment(textLayerLocation, GTextAlignmentCenter);
	text_layer_set_text_alignment(textLayerLocTime, GTextAlignmentLeft);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerTime));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerWeekDay));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerDay));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerTemp));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerTempHi));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerTempLo));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerLocation));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerLocTime));

	
  // Load the resource
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_45X45);
  
  sub_bitmap = gbitmap_create_as_sub_bitmap(s_bitmap,GRect(45,0,45,45));
  //sub_bitmap = gbitmap_create_as_sub_bitmap(s_bitmap,GRect(45,45,45,45));
  
  // Create the BitmapLayer
  s_bitmap_layer = bitmap_layer_create(GRect(1,168-45-LOC_HEIGHT-5,45,45));//5 boarder

  // Set the correct compositing mode
//  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
  bitmap_layer_set_bitmap(s_bitmap_layer, sub_bitmap);
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
   
  // Add to the Window
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));//
	
  window_stack_push(window, true);
	
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}


void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(textLayerTime);
	text_layer_destroy(textLayerWeekDay);
	text_layer_destroy(textLayerDay);
	text_layer_destroy(textLayerTemp);
	text_layer_destroy(textLayerTempHi);
	text_layer_destroy(textLayerTempLo);
	text_layer_destroy(textLayerLocation);
	text_layer_destroy(textLayerLocTime);
  
  gbitmap_destroy(s_bitmap);
  gbitmap_destroy(sub_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
	
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_weekDay_font);
  fonts_unload_custom_font(s_day_font);
  fonts_unload_custom_font(s_temp_font);
  fonts_unload_custom_font(s_temp_hl_font);
  fonts_unload_custom_font(s_location_font);
  fonts_unload_custom_font(s_locTime_font);
  
  // Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
