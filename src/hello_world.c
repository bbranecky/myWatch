#include <pebble.h>

#define TIME_HEIGHT       49

#define FONT_DAY          32//not used
#define DAY_HEIGHT        32 //no padding needed below
#define DAY_WIDTH         50

#define FONT_WEEK         30//not used
#define WEEK_TOP_PAD      5
#define WEEK_HEIGHT       30 //no padding needed below
#define WEEK_WIDTH        60

Window *window;
TextLayer *textLayerTime;
TextLayer *textLayerWeekDay;
TextLayer *textLayerDay;
TextLayer *text_layer3;

static GFont s_time_font;
static GFont s_weekDay_font;
static GFont s_day_font;
static GFont s_time_font2;

static void update_time() {
//  char text[50];

  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  static char timeStr[10];
  static char weekDayStr[15];
  static char dayStr[5];

  // Write the current hours and minutes into a buffer
  // Display this time on the TextLayer
  //strftime(timeStr, sizeof(timeStr), clock_is_24h_style() ? "%k:%M" : "%l:%M", tick_time); //%l no leading zero
  strftime(timeStr, sizeof(timeStr), "%l:%M", tick_time); //%l no leading zero
  //snprintf(timeStr, sizeof(timeStr), "%2d:%02d",(int)tick_time->tm_hour,(int)tick_time->tm_min );
  text_layer_set_text(textLayerTime, timeStr);
//strftime(timeStr, sizeof(timeStr), "%a, %b %e \n %l:%M ", tick_time); //mon day %a %b
	//APP_LOG(APP_LOG_LEVEL_DEBUG,"%s", timeStr);
  
  strftime(weekDayStr, sizeof(weekDayStr), "%a", tick_time); //%A no leading zero
//  text_layer_set_text(textLayerWeekDay, weekDayStr);
  //text_layer_set_text(textLayerWeekDay, "Wed");

  strftime(dayStr, sizeof(dayStr), "%e", tick_time); //%A no leading zero
//  text_layer_set_text(textLayerDay, dayStr);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
  //144 × 168
  textLayerTime = text_layer_create(GRect(0,0,144,TIME_HEIGHT)); //numbers only need to padding
  textLayerWeekDay = text_layer_create(GRect(144-DAY_WIDTH-WEEK_WIDTH, TIME_HEIGHT+WEEK_TOP_PAD, WEEK_WIDTH, WEEK_HEIGHT));
  textLayerDay = text_layer_create(GRect(144-DAY_WIDTH, TIME_HEIGHT, WEEK_WIDTH, DAY_HEIGHT));
  text_layer3 = text_layer_create(GRect(0, 49+FONT_DAY+5, 144, 37));
	
  // Apply to TextLayer 
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_25));
   
  //----time ---
  text_layer_set_font(textLayerTime, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));

  //--- week day: mon,tue... ---
  s_weekDay_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_30));
  text_layer_set_font(textLayerWeekDay, s_weekDay_font);  
  //text_layer_set_font(textLayerDate, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
 
  //----- Day #: 31 ------
  s_day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MANILA_SANS_BOLD_32));
  text_layer_set_font(textLayerDay, s_day_font);  

  text_layer_set_font(text_layer3, s_time_font);  
	
  text_layer_set_text(textLayerTime,"12:30");
  text_layer_set_text(textLayerWeekDay,"Wed");
  text_layer_set_text(textLayerDay,"23");
  text_layer_set_text(text_layer3,"text98)67");

  text_layer_set_text_alignment(textLayerTime, GTextAlignmentCenter);
	text_layer_set_text_alignment(textLayerWeekDay, GTextAlignmentCenter);
	text_layer_set_text_alignment(textLayerDay, GTextAlignmentCenter);
	text_layer_set_text_alignment(text_layer3, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerTime));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerWeekDay));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerDay));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer3));

	// Push the window
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
	text_layer_destroy(text_layer3);
	
// Unload GFont
fonts_unload_custom_font(s_time_font);
  
  // Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
