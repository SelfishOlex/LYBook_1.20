/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#include "precompiled.h"

#include <Components/Connections/DataConnections/DataConnectionGraphicsItem.h>

#include <GraphCanvas/Components/Slots/Data/DataSlotBus.h>
#include <GraphCanvas/Components/StyleBus.h>

namespace GraphCanvas
{
    ////////////////////////
    // DataPinStyleMonitor
    ////////////////////////

    DataPinStyleMonitor::DataPinStyleMonitor(DataConnectionGraphicsItem& graphicsItem)
        : m_graphicsItem(graphicsItem)
    {
    }

    void DataPinStyleMonitor::OnStyleChanged()
    {
        m_graphicsItem.UpdateDataColors();
    }

    void DataPinStyleMonitor::SetSourceId(const AZ::EntityId& sourceId)
    {
        if (m_sourceId != sourceId)
        {
            StyleNotificationBus::MultiHandler::BusDisconnect(m_sourceId);
            m_sourceId = sourceId;
            StyleNotificationBus::MultiHandler::BusConnect(m_sourceId);
        }
    }

    void DataPinStyleMonitor::SetTargetId(const AZ::EntityId& targetId)
    {
        if (m_targetId != targetId)
        {
            StyleNotificationBus::MultiHandler::BusDisconnect(m_targetId);
            m_targetId = targetId;
            StyleNotificationBus::MultiHandler::BusConnect(m_targetId);
        }
    }

    ///////////////////////////////
    // DataConnectionGraphicsItem
    ///////////////////////////////
    
    DataConnectionGraphicsItem::DataConnectionGraphicsItem(const AZ::EntityId& connectionEntityId)
        : ConnectionGraphicsItem(connectionEntityId)
        , m_dataPinStyleMonitor((*this))
    {
        RootGraphicsItemNotificationBus::Handler::BusConnect(connectionEntityId);
    }

    void DataConnectionGraphicsItem::OnStyleChanged()
    {
        ConnectionGraphicsItem::OnStyleChanged();

        UpdateDataColors();
    }

    void DataConnectionGraphicsItem::OnSourceSlotIdChanged(const AZ::EntityId& oldSlotId, const AZ::EntityId& newSlotId)
    {
        ConnectionGraphicsItem::OnSourceSlotIdChanged(oldSlotId, newSlotId);
        
        if (GetTargetSlotEntityId().IsValid())
        {
            m_sourceDataColor = m_targetDataColor;
        }
        
        m_dataPinStyleMonitor.SetSourceId(newSlotId);
        PopulateDataColor(m_sourceDataColor, newSlotId);
        UpdatePen();

        if (DataSlotNotificationBus::MultiHandler::BusIsConnectedId(oldSlotId))
        {
            DataSlotNotificationBus::MultiHandler::BusDisconnect(oldSlotId);
        }

        DataSlotNotificationBus::MultiHandler::BusConnect(newSlotId);
    }
    
    void DataConnectionGraphicsItem::OnTargetSlotIdChanged(const AZ::EntityId& oldSlotId, const AZ::EntityId& newSlotId)
    {
        ConnectionGraphicsItem::OnTargetSlotIdChanged(oldSlotId, newSlotId);
        
        if (GetSourceSlotEntityId().IsValid())
        {
            m_targetDataColor = m_sourceDataColor;
        }
        
        m_dataPinStyleMonitor.SetTargetId(newSlotId);
        PopulateDataColor(m_targetDataColor, newSlotId);
        UpdatePen();

        if (DataSlotNotificationBus::MultiHandler::BusIsConnectedId(oldSlotId))
        {
            DataSlotNotificationBus::MultiHandler::BusDisconnect(oldSlotId);
        }

        DataSlotNotificationBus::MultiHandler::BusConnect(newSlotId);
    }

    void DataConnectionGraphicsItem::OnDisplayTypeChanged(const AZ::Uuid&, const AZStd::vector<AZ::Uuid>&)
    {
        const AZ::EntityId* busId = DataSlotNotificationBus::GetCurrentBusId();

        if (busId == nullptr)
        {
            return;
        }

        if (GetSourceSlotEntityId() == (*busId))
        {
            PopulateDataColor(m_sourceDataColor, (*busId));
        }
        else if(GetTargetSlotEntityId() == (*busId))
        {
            PopulateDataColor(m_targetDataColor, (*busId));
        }

        UpdatePen();
    }

    void DataConnectionGraphicsItem::UpdateDataColors()
    {
        AZ::EntityId sourceSlotId = GetSourceSlotEntityId();
        AZ::EntityId targetSlotId = GetTargetSlotEntityId();

        if (sourceSlotId.IsValid() && targetSlotId.IsValid())
        {
            PopulateDataColor(m_sourceDataColor, sourceSlotId);
            PopulateDataColor(m_targetDataColor, targetSlotId);
        }
        else if (sourceSlotId.IsValid())
        {
            PopulateDataColor(m_sourceDataColor, sourceSlotId);
            PopulateDataColor(m_targetDataColor, sourceSlotId);
        }
        else if (targetSlotId.IsValid())
        {
            PopulateDataColor(m_sourceDataColor, targetSlotId);
            PopulateDataColor(m_targetDataColor, targetSlotId);
        }
        else
        {
            const Styling::StyleHelper& style = GetStyle();

            m_sourceDataColor = style.GetAttribute<QColor>(Styling::Attribute::LineColor);
            m_targetDataColor = style.GetAttribute<QColor>(Styling::Attribute::LineColor);
        }

        UpdatePen();
    }

    void DataConnectionGraphicsItem::OnDisplayStateChanged(RootGraphicsItemDisplayState, RootGraphicsItemDisplayState)
    {
        UpdatePen();
    }

    void DataConnectionGraphicsItem::UpdatePen()
    {
        ConnectionGraphicsItem::UpdatePen();

        if (!isSelected() && GetDisplayState() == RootGraphicsItemDisplayState::Neutral)
        {
            QLinearGradient gradient(path().pointAtPercent(0), path().pointAtPercent(1));

            gradient.setColorAt(0, m_sourceDataColor);
            gradient.setColorAt(1, m_targetDataColor);

            m_pen = pen();
            m_pen.setBrush(QBrush(gradient));

            setPen(m_pen);
        }
    }
	
    void DataConnectionGraphicsItem::OnPathChanged()
    {
        UpdatePen();
    }

    void DataConnectionGraphicsItem::PopulateDataColor(QColor& targetColor, const AZ::EntityId& slotId)
    {
        // Leave the color alone if we don't have a valid connection. Other logic deals with its coloring then.
        if (slotId.IsValid())
        {
            DataSlotType dataType = DataSlotType::Unknown;
            DataSlotRequestBus::EventResult(dataType, slotId, &DataSlotRequests::GetDataSlotType);

            const Styling::StyleHelper* stylingHelper = nullptr;
            
            if (dataType == DataSlotType::Container)
            {
                size_t typeCount = 0;
                DataSlotRequestBus::EventResult(typeCount, slotId, &DataSlotRequests::GetContainedTypesCount);

                if (typeCount == 1)
                {
                    // Vector/Array/Set
                    DataSlotRequestBus::EventResult(stylingHelper, slotId, &DataSlotRequests::GetContainedTypeColorPalette, 0);
                }
                else if(typeCount > 1)
                {
                    // Multi-Type container (e.g. Map)
                    DataSlotRequestBus::EventResult(stylingHelper, slotId, &DataSlotRequests::GetDataColorPalette);
                }
                else
                {
                    // Container with no contained types (e.g. dynamic container slot)
                    DataSlotRequestBus::EventResult(stylingHelper, slotId, &DataSlotRequests::GetDataColorPalette);
                }
            }
            else
            {
                DataSlotRequestBus::EventResult(stylingHelper, slotId, &DataSlotRequests::GetDataColorPalette);
            }

            if (stylingHelper != nullptr)
            {
                targetColor = stylingHelper->GetColor(Styling::Attribute::LineColor);
            }
            else
            {
                targetColor = Qt::white;
            }
        }
    }    
}