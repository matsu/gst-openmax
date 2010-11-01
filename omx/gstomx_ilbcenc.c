/*
 * Copyright (C) 2007-2009 Nokia Corporation.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "gstomx_ilbcenc.h"
#include "gstomx_base_filter.h"
#include "gstomx.h"

GSTOMX_BOILERPLATE (GstOmxIlbcEnc, gst_omx_ilbcenc, GstOmxBaseFilter,
    GST_OMX_BASE_FILTER_TYPE);

static void
type_base_init (gpointer g_class)
{
  GstElementClass *element_class;

  element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_set_details_simple (element_class,
      "OpenMAX IL iLBC audio encoder",
      "Codec/Encoder/Audio",
      "Encodes audio in iLBC format with OpenMAX IL", "Felipe Contreras");

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "sink")));

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "src")));
}

static void
type_class_init (gpointer g_class, gpointer class_data)
{
}

static gboolean
sink_setcaps (GstPad * pad, GstCaps * caps)
{
  GstOmxBaseFilter *omx_base;

  omx_base = GST_OMX_BASE_FILTER (GST_PAD_PARENT (pad));

  GST_INFO_OBJECT (omx_base, "setcaps (sink): %" GST_PTR_FORMAT, caps);

  /* set caps on the srcpad */
  {
    GstCaps *tmp_caps;

    tmp_caps = gst_pad_get_allowed_caps (omx_base->srcpad);
    tmp_caps = gst_caps_make_writable (tmp_caps);
    gst_caps_truncate (tmp_caps);

    gst_pad_fixate_caps (omx_base->srcpad, tmp_caps);

    if (gst_caps_is_fixed (tmp_caps)) {
      GST_INFO_OBJECT (omx_base, "fixated to: %" GST_PTR_FORMAT, tmp_caps);
      gst_pad_set_caps (omx_base->srcpad, tmp_caps);
    }

    gst_caps_unref (tmp_caps);
  }

  return gst_pad_set_caps (pad, caps);
}

static void
type_instance_init (GTypeInstance * instance, gpointer g_class)
{
  GstOmxBaseFilter *omx_base;

  omx_base = GST_OMX_BASE_FILTER (instance);

  gst_pad_set_setcaps_function (omx_base->sinkpad, sink_setcaps);
}
