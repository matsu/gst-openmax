/*
 * Copyright (C) 2007-2008 Nokia Corporation. All rights reserved.
 *
 * Author: Felipe Contreras <felipe.contreras@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "gstomx_base_videoenc.h"
#include "gstomx.h"

static GstOmxBaseFilterClass *parent_class = NULL;

static GstCaps *
generate_sink_template ()
{
    GstCaps *caps;
    GstStructure *struc;

    caps = gst_caps_new_empty ();

    struc = gst_structure_new ("video/x-raw-yuv",
                               "width", GST_TYPE_INT_RANGE, 16, 4096,
                               "height", GST_TYPE_INT_RANGE, 16, 4096,
                               "framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 30, 1,
                               NULL);

    {
        GValue list = { 0 };
        GValue val = { 0 };

        g_value_init (&list, GST_TYPE_LIST);
        g_value_init (&val, GST_TYPE_FOURCC);

        gst_value_set_fourcc (&val, GST_MAKE_FOURCC ('I', '4', '2', '0'));
        gst_value_list_append_value (&list, &val);

#if 0
        gst_value_set_fourcc (&val, GST_MAKE_FOURCC ('Y', 'U', 'Y', '2'));
        gst_value_list_append_value (&list, &val);

        gst_value_set_fourcc (&val, GST_MAKE_FOURCC ('U', 'Y', 'V', 'Y'));
        gst_value_list_append_value (&list, &val);
#endif

        gst_structure_set_value (struc, "format", &list);

        g_value_unset (&val);
        g_value_unset (&list);
    }

    gst_caps_append_structure (caps, struc);

    return caps;
}

static void
type_base_init (gpointer g_class)
{
    GstElementClass *element_class;

    element_class = GST_ELEMENT_CLASS (g_class);

    {
        GstPadTemplate *template;

        template = gst_pad_template_new ("sink", GST_PAD_SINK,
                                         GST_PAD_ALWAYS,
                                         generate_sink_template ());

        gst_element_class_add_pad_template (element_class, template);
    }
}

static void
type_class_init (gpointer g_class,
                 gpointer class_data)
{
    parent_class = g_type_class_ref (GST_OMX_BASE_FILTER_TYPE);
}

static gboolean
sink_setcaps (GstPad *pad,
              GstCaps *caps)
{
    GstStructure *structure;
    GstOmxBaseFilter *omx_base;
    GOmxCore *gomx;
    OMX_COLOR_FORMATTYPE color_format;
    gint width = 0;
    gint height = 0;
    gint framerate = 0;

    omx_base = GST_OMX_BASE_FILTER (GST_PAD_PARENT (pad));
    gomx = (GOmxCore *) omx_base->gomx;

    GST_INFO_OBJECT (omx_base, "setcaps (sink): %" GST_PTR_FORMAT, caps);

    g_return_val_if_fail (gst_caps_get_size (caps) == 1, FALSE);

    structure = gst_caps_get_structure (caps, 0);

    gst_structure_get_int (structure, "width", &width);
    gst_structure_get_int (structure, "height", &height);

    {
        const GValue *tmp;
        tmp = gst_structure_get_value (structure, "framerate");

        if (tmp != NULL)
        {
            framerate = gst_value_get_fraction_numerator (tmp) / gst_value_get_fraction_denominator (tmp);
        }

    }

    if (strcmp (gst_structure_get_name (structure), "video/x-raw-yuv") == 0)
    {
        guint32 fourcc;

        if (gst_structure_get_fourcc (structure, "format", &fourcc))
        {
            switch (fourcc)
            {
                case GST_MAKE_FOURCC ('I', '4', '2', '0'):
                    color_format = OMX_COLOR_FormatYUV420Planar;
                    break;
                case GST_MAKE_FOURCC ('Y', 'U', 'Y', '2'):
                    color_format = OMX_COLOR_FormatYCbYCr;
                    break;
            }
        }
    }

    {
        OMX_PARAM_PORTDEFINITIONTYPE *param;
        param = calloc (1, sizeof (OMX_PARAM_PORTDEFINITIONTYPE));
        param->nSize = sizeof (OMX_PARAM_PORTDEFINITIONTYPE);
        param->nVersion.s.nVersionMajor = 1;
        param->nVersion.s.nVersionMinor = 1;

        /* Input port configuration. */
        {
            param->nPortIndex = 0;
            OMX_GetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, param);

            /** @todo properly read this from properties */
            param->format.video.nBitrate = 512000;

            param->format.video.nFrameWidth = width;
            param->format.video.nFrameHeight = height;
            param->format.video.xFramerate = framerate;
            param->format.video.eColorFormat = color_format;

            OMX_SetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, param);
        }

        free (param);
    }

    return gst_pad_set_caps (pad, caps);
}

static void
omx_setup (GstOmxBaseFilter *omx_base)
{
    GstOmxBaseVideoEnc *self;
    GOmxCore *gomx;

    self = GST_OMX_BASE_VIDEOENC (omx_base);
    gomx = (GOmxCore *) omx_base->gomx;

    GST_INFO_OBJECT (omx_base, "begin");

    {
        OMX_PARAM_PORTDEFINITIONTYPE *param;
        OMX_COLOR_FORMATTYPE color_format;
        gint width, height;
        guint framerate;

        param = calloc (1, sizeof (OMX_PARAM_PORTDEFINITIONTYPE));
        param->nSize = sizeof (OMX_PARAM_PORTDEFINITIONTYPE);
        param->nVersion.s.nVersionMajor = 1;
        param->nVersion.s.nVersionMinor = 1;

        /* Output port configuration. */
        {
            param->nPortIndex = 1;
            OMX_GetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, param);

            param->format.video.eCompressionFormat = self->compression_format;

            OMX_SetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, param);
        }

        /* TI specific hacks. */
#if 0
        {
            param->nPortIndex = 0;
            OMX_GetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, param);

            width = param->format.video.nFrameWidth;
            height = param->format.video.nFrameHeight;
            framerate = param->format.video.xFramerate;
            color_format = param->format.video.eColorFormat;

            /* this is against the standard; nBufferSize is read-only. */
            switch (color_format)
            {
                case OMX_COLOR_FormatYCbYCr:
                case OMX_COLOR_FormatCbYCrY:
                    param->nBufferSize = (width * height) * 2;
                    break;
                case OMX_COLOR_FormatYUV420Planar:
                    param->nBufferSize = (width * height) * 1.5;
                    break;
                default:
                    break;
            }

            OMX_SetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, param);
        }

        /* the component should do this instead */
        {
            param->nPortIndex = 1;
            OMX_GetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, param);

            param->nBufferSize = (width * height) / 2;

            param->format.video.nFrameWidth = width;
            param->format.video.nFrameHeight = height;
            param->format.video.xFramerate = framerate;

            OMX_SetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, param);
        }
#endif

        free (param);
    }

    GST_INFO_OBJECT (omx_base, "end");
}

static void
type_instance_init (GTypeInstance *instance,
                    gpointer g_class)
{
    GstOmxBaseFilter *omx_base;

    omx_base = GST_OMX_BASE_FILTER (instance);

    omx_base->omx_setup = omx_setup;

    gst_pad_set_setcaps_function (omx_base->sinkpad, sink_setcaps);
}

GType
gst_omx_base_videoenc_get_type (void)
{
    static GType type = 0;

    if (type == 0)
    {
        GTypeInfo *type_info;

        type_info = g_new0 (GTypeInfo, 1);
        type_info->class_size = sizeof (GstOmxBaseVideoEncClass);
        type_info->base_init = type_base_init;
        type_info->class_init = type_class_init;
        type_info->instance_size = sizeof (GstOmxBaseVideoEnc);
        type_info->instance_init = type_instance_init;

        type = g_type_register_static (GST_OMX_BASE_FILTER_TYPE, "GstOmxBaseVideoEnc", type_info, 0);

        g_free (type_info);
    }

    return type;
}
