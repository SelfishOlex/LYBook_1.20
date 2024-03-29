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

#pragma once

#include <ScriptCanvas/Core/Node.h>
#include <ScriptCanvas/Core/Graph.h>
#include <ScriptCanvas/Core/Datum.h>
#include <ScriptCanvas/Data/PropertyTraits.h>

namespace AZ
{
    class BehaviorClass;
    struct BehaviorValueParameter;
    class ReflectContext;
}

namespace ScriptCanvas
{
    struct PropertyAccount
    {
        AZStd::unordered_map<SlotId, Data::GetterWrapper> m_gettersByInputSlot;
        AZStd::unordered_map<SlotId, Data::SetterWrapper> m_settersByInputSlot;
        // The first slot id of the pair is the Getter SlotId, the second slot id of the pair is the Setter SlotID
        AZStd::unordered_map<AZStd::string, AZStd::pair<SlotId, SlotId>> m_getterSetterIdPairs;
    };

    class PureData
        : public Node
    {
    public:
        AZ_COMPONENT(PureData, "{8B80FF54-0786-4FEE-B4A3-12907EBF8B75}", Node);

        static void Reflect(AZ::ReflectContext* reflectContext);
        static const char* k_getThis;
        static const char* k_setThis;

        const AZStd::unordered_map<AZStd::string, AZStd::pair<SlotId, SlotId>>& GetPropertyNameSlotMap() const;

        ~PureData() override;

    protected:
        void AddInputAndOutputTypeSlot(const Data::Type& type, const void* defaultValue = nullptr);
        template<typename DatumType>
        void AddDefaultInputAndOutputTypeSlot(DatumType&& defaultValue);
        void AddInputTypeAndOutputTypeSlot(const Data::Type& type);
        
        void OnActivate() override;
        void OnInputChanged(const Datum& input, const SlotId& id) override;
        void MarkDefaultableInput() override {}

        AZ_INLINE void OnOutputChanged(const Datum& output) const
        { 
            auto slotNameIter = m_slotNameMap.find(GetOutputDataName());
            if (slotNameIter != m_slotNameMap.end())
            {
                OnOutputChanged(output, *slotNameIter->second);
            }
        }
        
        AZ_INLINE void OnOutputChanged(const Datum& output, const Slot& outputSlot) const
        {
            PushOutput(output, outputSlot);
        }

        // push data out
        AZ_INLINE void PushThis()
        {
            auto slotId = GetSlotId(GetInputDataName());

            if (auto setDatum = GetInput(slotId))
            {
                OnInputChanged(*setDatum, slotId);
            }
            else
            {
                SCRIPTCANVAS_REPORT_ERROR((*this), "No input datum in a PureData class %s. You must push your data manually in OnActivate() if no input is connected!");
            }
        }

        AZStd::string_view GetInputDataName() const;
        AZStd::string_view GetOutputDataName() const;

        void SetInput(const Datum& input, const SlotId& id) override;
        void SetInput(Datum&& input, const SlotId& id) override;
        void SetProperty(const Datum& input, const SlotId& id);
        void CallGetter(const SlotId& getterSlotId);
        bool IsConfigured() { return m_configured; }

        void Visit(NodeVisitor& visitor) const override { visitor.Visit(*this); }

        PropertyAccount m_propertyAccount;
        bool m_configured = false;
    };

    template<typename DatumType>
    void PureData::AddDefaultInputAndOutputTypeSlot(DatumType&& defaultValue)
    {
        AddInputDatumSlot(GetInputDataName(), "", Datum::eOriginality::Original, AZStd::forward<DatumType>(defaultValue));
        AddOutputTypeSlot(GetOutputDataName(), "", Data::FromAZType(azrtti_typeid<AZStd::decay_t<DatumType>>()), OutputStorage::Optional);
    }

    template<>
    void PureData::AddDefaultInputAndOutputTypeSlot<Data::Type>(Data::Type&&) = delete;
}