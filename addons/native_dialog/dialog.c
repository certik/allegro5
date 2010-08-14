#include "allegro5/allegro.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/internal/aintern_native_dialog.h"
#include "allegro5/internal/aintern_dtor.h"
#include "allegro5/internal/aintern_system.h"


/* Function: al_create_native_file_dialog
 */
ALLEGRO_FILECHOOSER *al_create_native_file_dialog(
   ALLEGRO_PATH const *initial_path,
   char const *title,
   char const *patterns,
   int mode)
{
   ALLEGRO_NATIVE_DIALOG *fc;
   fc = al_malloc(sizeof *fc);
   memset(fc, 0, sizeof *fc);

   if (initial_path)
      fc->fc_initial_path = al_clone_path(initial_path);
   fc->title = al_ustr_new(title);
   fc->fc_patterns = al_ustr_new(patterns);
   fc->flags = mode;

   _al_register_destructor(_al_dtor_list, fc,
      (void (*)(void *))al_destroy_native_file_dialog);

   return (ALLEGRO_FILECHOOSER *)fc;
}

/* Function: al_show_native_file_dialog
 */
bool al_show_native_file_dialog(ALLEGRO_DISPLAY *display,
   ALLEGRO_FILECHOOSER *dialog)
{
   ALLEGRO_NATIVE_DIALOG *fd = (ALLEGRO_NATIVE_DIALOG *)dialog;
   return _al_show_native_file_dialog(display, fd);
}

/* Function: al_get_native_file_dialog_count
 */
int al_get_native_file_dialog_count(const ALLEGRO_FILECHOOSER *dialog)
{
   const ALLEGRO_NATIVE_DIALOG *fc = (const ALLEGRO_NATIVE_DIALOG *)dialog;
   return fc->fc_path_count;
}

/* Function: al_get_native_file_dialog_path
 */
const ALLEGRO_PATH *al_get_native_file_dialog_path(
   const ALLEGRO_FILECHOOSER *dialog, size_t i)
{
   const ALLEGRO_NATIVE_DIALOG *fc = (const ALLEGRO_NATIVE_DIALOG *)dialog;
   if (i < fc->fc_path_count)
      return fc->fc_paths[i];
   return NULL;
}

/* Function: al_destroy_native_file_dialog
 */
void al_destroy_native_file_dialog(ALLEGRO_FILECHOOSER *dialog)
{
   ALLEGRO_NATIVE_DIALOG *fd = (ALLEGRO_NATIVE_DIALOG *)dialog;
   size_t i;

   if (!fd)
      return;

   _al_unregister_destructor(_al_dtor_list, fd);

   al_ustr_free(fd->title);
   al_destroy_path(fd->fc_initial_path);
   for (i = 0; i < fd->fc_path_count; i++) {
      al_destroy_path(fd->fc_paths[i]);
   }
   al_free(fd->fc_paths);
   al_ustr_free(fd->fc_patterns);
   al_free(fd);
}

/* Function: al_show_native_message_box
 */
int al_show_native_message_box(ALLEGRO_DISPLAY *display,
   char const *title, char const *heading, char const *text,
   char const *buttons, int flags)
{
   ALLEGRO_NATIVE_DIALOG *fc;
   int r;

   fc = al_malloc(sizeof *fc);
   memset(fc, 0, sizeof *fc);

   fc->title = al_ustr_new(title);
   fc->mb_heading = al_ustr_new(heading);
   fc->mb_text = al_ustr_new(text);
   fc->mb_buttons = al_ustr_new(buttons);
   fc->flags = flags;

   r = _al_show_native_message_box(display, fc);

   al_ustr_free(fc->title);
   al_ustr_free(fc->mb_heading);
   al_ustr_free(fc->mb_text);
   al_ustr_free(fc->mb_buttons);
   al_free(fc);

   return r;
}


/* Function: al_get_allegro_native_dialog_version
 */
uint32_t al_get_allegro_native_dialog_version(void)
{
   return ALLEGRO_VERSION_INT;
}


/* vim: set sts=3 sw=3 et: */
