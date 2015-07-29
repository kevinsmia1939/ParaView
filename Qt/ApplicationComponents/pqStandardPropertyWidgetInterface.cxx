/*=========================================================================

   Program: ParaView
   Module: pqStandardPropertyWidgetInterface.cxx

   Copyright (c) 2012 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "pqStandardPropertyWidgetInterface.h"

#include "pqArrayStatusPropertyWidget.h"
#include "pqBackgroundEditorWidget.h"
#include "pqCalculatorWidget.h"
#include "pqCameraManipulatorWidget.h"
#include "pqColorAnnotationsPropertyWidget.h"
#include "pqColorEditorPropertyWidget.h"
#include "pqColorOpacityEditorWidget.h"
#include "pqColorPaletteSelectorWidget.h"
#include "pqColorSelectorPropertyWidget.h"
#include "pqCommandButtonPropertyWidget.h"
#include "pqCTHArraySelectionDecorator.h"
#include "pqCubeAxesPropertyWidget.h"
#include "pqDisplayRepresentationWidget.h"
#include "pqDoubleRangeSliderPropertyWidget.h"
#include "pqEnableWidgetDecorator.h"
#include "pqFileNamePropertyWidget.h"
#include "pqFontPropertyWidget.h"
#include "pqGenericPropertyWidgetDecorator.h"
#include "pqGlyphScaleFactorPropertyWidget.h"
#include "pqImageCompressorWidget.h"
#include "pqInputDataTypeDecorator.h"
#include "pqIntMaskPropertyWidget.h"
#include "pqLightsEditor.h"
#include "pqListPropertyWidget.h"
#include "pqPropertyGroupButton.h"
#include "pqProxyEditorPropertyWidget.h"
#include "pqSeriesEditorPropertyWidget.h"
#include "pqShowWidgetDecorator.h"
#include "pqTextLocationWidget.h"
#include "pqTextureSelectorPropertyWidget.h"
#include "pqTransferFunctionWidgetPropertyWidget.h"
#include "pqViewTypePropertyWidget.h"
#include "vtkSMPropertyGroup.h"
#include "vtkSMProperty.h"

#include <QtDebug>

//-----------------------------------------------------------------------------
pqStandardPropertyWidgetInterface::pqStandardPropertyWidgetInterface(QObject *p)
  : QObject(p)
{
}

//-----------------------------------------------------------------------------
pqStandardPropertyWidgetInterface::~pqStandardPropertyWidgetInterface()
{
}

//-----------------------------------------------------------------------------
pqPropertyWidget*
pqStandardPropertyWidgetInterface::createWidgetForProperty(vtkSMProxy *smProxy,
                                                           vtkSMProperty *smProperty)
{
  // handle properties that specify custom panel widgets
  const char *custom_widget = smProperty->GetPanelWidget();
  if (!custom_widget)
    {
    return NULL;
    }

  std::string name = custom_widget;

  // *** NOTE: When adding new types, please update the header documentation ***
  if (name == "color_palette_selector")
    {
    return new pqColorPaletteSelectorWidget(smProxy, smProperty);
    }
  else if(name == "color_selector")
    {
    bool withPalette = false;
    return new pqColorSelectorPropertyWidget(smProxy, smProperty, withPalette);
    }
  else if(name == "color_selector_with_palette")
    {
    bool withPalette = true;
    return new pqColorSelectorPropertyWidget(smProxy, smProperty, withPalette);
    }
  else if(name == "display_representation_selector")
    {
    return new pqDisplayRepresentationPropertyWidget(smProxy);
    }
  else if(name == "texture_selector")
    {
    return new pqTextureSelectorPropertyWidget(smProxy);
    }
  else if (name == "calculator")
    {
    return new pqCalculatorWidget(smProxy, smProperty);
    }
  else if(name == "command_button")
    {
    return new pqCommandButtonPropertyWidget(smProxy, smProperty);
    }
  else if(name == "transfer_function_editor")
    {
    return new pqTransferFunctionWidgetPropertyWidget(smProxy, smProperty);
    }
  else if (name == "list")
    {
    return new pqListPropertyWidget(smProxy, smProperty);
    }
  else if (name == "double_range")
    {
    return new pqDoubleRangeSliderPropertyWidget(smProxy, smProperty);
    }
  else if (name == "image_compressor_config")
    {
    return new pqImageCompressorWidget(smProxy, smProperty);
    }
  else if (name == "camera_manipulator")
    {
    return new pqCameraManipulatorWidget(smProxy, smProperty);
    }
  else if (name == "viewtype_selector")
    {
    return new pqViewTypePropertyWidget(smProxy, smProperty);
    }
  else if (name == "glyph_scale_factor")
    {
    return new pqGlyphScaleFactorPropertyWidget(smProxy, smProperty);
    }
  else if (name == "proxy_editor")
    {
    return new pqProxyEditorPropertyWidget(smProxy, smProperty);
    }
  else if (name == "int_mask")
    {
    return new pqIntMaskPropertyWidget(smProxy, smProperty);
    }
  else if (name == "filename_annotation")
    {
    return new pqFileNamePropertyWidget(smProxy, smProperty);
    }
  // *** NOTE: When adding new types, please update the header documentation ***
  return NULL;
}

//-----------------------------------------------------------------------------
pqPropertyWidget*
pqStandardPropertyWidgetInterface::createWidgetForPropertyGroup(
  vtkSMProxy *proxy,
  vtkSMPropertyGroup *group)
{
  // *** NOTE: When adding new types, please update the header documentation ***
  if(QString(group->GetPanelWidget()) == "ColorEditor")
    {
    return new pqColorEditorPropertyWidget(proxy);
    }
  else if(QString(group->GetPanelWidget()) == "CubeAxes")
    {
    return new pqCubeAxesPropertyWidget(proxy);
    }
  else if(QString(group->GetPanelWidget()) == "BackgroundEditor")
    {
    return new pqBackgroundEditorWidget(proxy, group);
    }
  else if(QString(group->GetPanelWidget()) == "LightsEditor")
    {
    pqPropertyGroupButton * pgb = new pqPropertyGroupButton(proxy, group);
    pgb->SetEditor (new pqLightsEditor(pgb));
    return pgb;
    }
  else if (QString(group->GetPanelWidget()) == "ArrayStatus")
    {
    return new pqArrayStatusPropertyWidget(proxy, group);
    }
  else if (QString(group->GetPanelWidget()) == "ColorOpacityEditor")
    {
    return new pqColorOpacityEditorWidget(proxy, group);
    }
  else if (QString(group->GetPanelWidget()) == "AnnotationsEditor")
    {
    return new pqColorAnnotationsPropertyWidget(proxy, group);
    }
  else if (QString(group->GetPanelWidget()) == "FontEditor")
    {
    return new pqFontPropertyWidget(proxy, group);
    }
  else if (QString(group->GetPanelWidget()) == "SeriesEditor")
    {
    return new pqSeriesEditorPropertyWidget(proxy, group);
    }
  else if (QString(group->GetPanelWidget()) == "TextLocationEditor")
    {
    return new pqTextLocationWidget(proxy, group);
    }
  // *** NOTE: When adding new types, please update the header documentation ***

  return 0;
}

//-----------------------------------------------------------------------------
pqPropertyWidgetDecorator*
pqStandardPropertyWidgetInterface::createWidgetDecorator(
  const QString& type, vtkPVXMLElement* config, pqPropertyWidget* widget)
{
  // *** NOTE: When adding new types, please update the header documentation ***
  if (type == "CTHArraySelectionDecorator")
    {
    return new pqCTHArraySelectionDecorator(config, widget);
    }
  if (type == "InputDataTypeDecorator")
    {
    return new pqInputDataTypeDecorator(config, widget);
    }
  if (type == "EnableWidgetDecorator")
    {
    return new pqEnableWidgetDecorator(config, widget);
    }
  if (type == "ShowWidgetDecorator")
    {
    return new pqShowWidgetDecorator(config, widget);
    }
  if (type == "GenericDecorator")
    {
    return new pqGenericPropertyWidgetDecorator(config, widget);
    }

  // *** NOTE: When adding new types, please update the header documentation ***
  return NULL;
}
