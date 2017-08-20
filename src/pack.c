/*
 * Copyright (c) 2017 Jean Guyomarc'h
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "Envim.h"

/* FIXME. Ideally, these values should be dynamic (reading the API contract) */
typedef enum
{
  E_NVIM_OBJECT_BUFFER = 1,
  E_NVIM_OBJECT_WINDOW = 2,
  E_NVIM_OBJECT_TAB = 3,
} e_nvim_object;

static inline t_int
_read_object_id(const msgpack_object_ext *obj)
{
   t_int id = 0;
   memcpy(&id, obj->ptr, obj->size);
   return id;
}

/*============================================================================*
 *                                 Packing API                                *
 *============================================================================*/

void
pack_non_implemented(msgpack_packer *pk EINA_UNUSED,
                     const void *obj EINA_UNUSED)
{
   CRI("This is unimplemented. Type is too generic to be packed");
}

void
pack_boolean(msgpack_packer *pk,
             Eina_Bool boolean)
{
   if (boolean == EINA_FALSE) msgpack_pack_false(pk);
   else msgpack_pack_true(pk);
}

void
pack_stringshare(msgpack_packer *pk,
                 Eina_Stringshare *str)
{
   const size_t len = (size_t)eina_stringshare_strlen(str);
   msgpack_pack_bin(pk, len);
   msgpack_pack_bin_body(pk, str, len);
}

void
pack_position(msgpack_packer *pk,
              s_position pos)
{
   msgpack_pack_array(pk, 2);
   msgpack_pack_int64(pk, pos.x);
   msgpack_pack_int64(pk, pos.y);
}

static void
_pack_list_of_objects(msgpack_packer *pk,
                      const Eina_List *list)
{
   Eina_List *l;
   const t_int *id;

   EINA_LIST_FOREACH(list, l, id)
      msgpack_pack_int64(pk, *id);
}

void
pack_list_of_windows(msgpack_packer *pk,
                     const Eina_List *list_win)
{
   _pack_list_of_objects(pk, list_win);
}

void
pack_list_of_buffers(msgpack_packer *pk,
                     const Eina_List *list_buf)
{
   _pack_list_of_objects(pk, list_buf);
}

void
pack_list_of_tabpages(msgpack_packer *pk,
                      const Eina_List *list_tab)
{
   _pack_list_of_objects(pk, list_tab);
}

void
pack_list_of_strings(msgpack_packer *pk,
                     const Eina_List *list_str)
{
   Eina_List *l;
   Eina_Stringshare *str;

   EINA_LIST_FOREACH(list_str, l, str)
      pack_stringshare(pk, str);
}


/*============================================================================*
 *                                Unpacking API                               *
 *============================================================================*/

#define ARGS_CHECK_SIZE(ARGS, SIZE, RET)                                       \
   if (EINA_UNLIKELY(ARGS->size != (SIZE)))                                    \
     {                                                                         \
        ERR("Array contains %"PRIu32" elements instead of %u", ARGS->size, SIZE); \
        return RET;                                                            \
     }

Eina_Bool
pack_boolean_get(const msgpack_object_array *args)
{
   ARGS_CHECK_SIZE(args, 1, EINA_FALSE);
   if (EINA_UNLIKELY(args->ptr[0].type != MSGPACK_OBJECT_BOOLEAN))
     {
        ERR("Object does not contain a boolean value");
        return EINA_FALSE;
     }
   return (args->ptr[0].via.boolean == 0) ? EINA_FALSE : EINA_TRUE;
}

s_position
pack_position_get(const msgpack_object_array *args)
{
   ARGS_CHECK_SIZE(args, 2, position_make(-1, -1));

   if (EINA_UNLIKELY((args->ptr[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER) &&
                     (args->ptr[1].type != MSGPACK_OBJECT_POSITIVE_INTEGER)))
     {
        ERR("Object does not contain two positive integers");
        return position_make(-1, -1);
     }
   return position_make(args->ptr[0].via.i64, args->ptr[1].via.i64);
}

t_int
pack_int_get(const msgpack_object_array *args)
{
   ARGS_CHECK_SIZE(args, 1, 0);

   if (EINA_UNLIKELY((args->ptr[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER) &&
                     (args->ptr[0].type != MSGPACK_OBJECT_NEGATIVE_INTEGER)))
     {
        ERR("Object does not contain an integer");
        return 0;
     }
   return args->ptr[0].via.i64;
}

Eina_Stringshare *
pack_stringshare_get(const msgpack_object_array *args)
{
   ARGS_CHECK_SIZE(args, 1, NULL);

   if (EINA_UNLIKELY(args->ptr[0].type != MSGPACK_OBJECT_STR))
     {
        ERR("Object does not contain a string");
        return NULL;
     }
   const msgpack_object_str *const str = &(args->ptr[0].via.str);
   return eina_stringshare_add_length(str->ptr, str->size);
}

t_int
pack_object_get(const msgpack_object_array *args)
{
   CRI("Unimplemented"); (void) args;
   return T_INT_INVALID;
}

t_int
pack_window_get(const msgpack_object_array *args)
{
   CRI("Unimplemented"); (void) args;
   return T_INT_INVALID;
}

t_int
pack_buffer_get(const msgpack_object_array *args)
{
   ARGS_CHECK_SIZE(args, 1, T_INT_INVALID);

   if (EINA_UNLIKELY(args->ptr[0].type != MSGPACK_OBJECT_EXT))
     {
        ERR("Response type 0x%x is not an EXT type", args->ptr[0].type);
        return T_INT_INVALID;
     }

   const msgpack_object_ext *const obj = &(args->ptr[0].via.ext);
   if (EINA_UNLIKELY(obj->type != E_NVIM_OBJECT_BUFFER))
     {
        ERR("Subtype 0x%x is not a NeoVim Buffer", obj->type);
        return T_INT_INVALID;
     }

   return _read_object_id(obj);
}

t_int
pack_tabpage_get(const msgpack_object_array *args)
{
   CRI("Unimplemented"); (void) args;
   return T_INT_INVALID;
}

static Eina_List *
_pack_list_of_objects_get(const msgpack_object_array *args)
{
   Eina_List *list = NULL;

   for (unsigned int i = 0; i < args->size; i++)
     {
        if (EINA_UNLIKELY(args->ptr[i].type != MSGPACK_OBJECT_EXT))
          {
             ERR("Expected MSGPACK_OBJECT_EXT type");
             goto fail;
          }

        const msgpack_object_ext *const obj = &(args->ptr[i].via.ext);
        const t_int id = _read_object_id(obj);
        INF("size: %"PRIu32", %02x %02x %02x => %"PRIx64, obj->size,
            (unsigned char)obj->ptr[0], (unsigned char)obj->ptr[1], (unsigned char)obj->ptr[2], id);

        list = eina_list_append(list, (const void *)id);
     }
   return list;

fail:
   eina_list_free(list);
   return NULL;
}

Eina_List *
pack_tabpages_get(const msgpack_object_array *args)
{
   return _pack_list_of_objects_get(args);
}

Eina_List *
pack_windows_get(const msgpack_object_array *args)
{
   return _pack_list_of_objects_get(args);
}

Eina_List *
pack_buffers_get(const msgpack_object_array *args)
{
   return _pack_list_of_objects_get(args);
}

Eina_List *
pack_strings_get(const msgpack_object_array *args)
{
   Eina_List *list = NULL;

   for (unsigned int i = 0; i < args->size; i++)
     {
        /* We expect to receive strigns only. Skip non-strings with an error
         * message, but keep going */
        if (args->ptr[i].type != MSGPACK_OBJECT_STR)
          {
             ERR("Expected EXT type but got 0x%x", args->ptr[i].type);
             continue;
          }
     //   const msgpack_object_ext *const ext = &(args->ptr[i].via.ext);
     //   CRI("Subtype: 0x%x\n", ext->type);
     //   continue;

        /* Create a stringshare from the msgpack string */
        const msgpack_object_str *const s = &(args->ptr[i].via.str);
        Eina_Stringshare *const str = eina_stringshare_add_length(s->ptr, s->size);
        if (EINA_UNLIKELY(! str))
          {
             CRI("Failed to create stringshare");
             continue;
          }

        /* And finally store it in the list */
        list = eina_list_append(list, str);
     }

   return list;
}

void *
pack_non_implemented_get(const msgpack_object_array *args EINA_UNUSED)
{
   CRI("Non implemented");
   return NULL;
}
