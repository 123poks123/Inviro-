/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MaggsA3AudioProcessor::MaggsA3AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  AudioChannelSet::octagonal() , true)
#endif
                  .withOutput ("Output", AudioChannelSet::octagonal(), true)
#endif
                  ), parameters(*this, nullptr, Identifier("Parameters"),
                                
                                {
                                    //add that list of parameters that you think the plugin will have
                                    std::make_unique<AudioParameterFloat>("panx" , //parameter ID
                                                                          "PanX" , //Parameter Name
                                                                          -100.0f, //min value
                                                                          100.0f, //Max Value
                                                                          0.0f //Default Value
                                                                          ),
                                    
                                    std::make_unique<AudioParameterFloat>("pany" , //parameter ID
                                                                          "PanY" , //Parameter Name
                                                                          -100.0f, //min value
                                                                          100.0f, //Max Value
                                                                          0.0f //Default Value
                                                                          ),
                                    
                                    std::make_unique<AudioParameterFloat>("outputmenu" , //parameter ID
                                                                          "output" , //Parameter Name
                                                                          0.0f, //min value
                                                                          5.0f, //Max Value
                                                                          0.0f //Default Value
                                                                          ),
                                    
                                    std::make_unique<AudioParameterFloat>("inputmenu" , //parameter ID
                                                                          "input" , //Parameter Name
                                                                          0.0f, //min value
                                                                          2.0f, //Max Value
                                                                          0.0f //Default Value
                                                                          )
                                    
                                    
                                }
                                )

#endif
{
    panParameter = parameters.getRawParameterValue("panx");
    
    panY = parameters.getRawParameterValue("pany");
    
    outputMenu = parameters.getRawParameterValue("outputmenu");
    
    inputMenu = parameters.getRawParameterValue("inputmenu");
    
    
}

MaggsA3AudioProcessor::~MaggsA3AudioProcessor()
{
}

//==============================================================================
const String MaggsA3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MaggsA3AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MaggsA3AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MaggsA3AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MaggsA3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MaggsA3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int MaggsA3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void MaggsA3AudioProcessor::setCurrentProgram (int index)
{
}

const String MaggsA3AudioProcessor::getProgramName (int index)
{
    return {};
}

void MaggsA3AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MaggsA3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    
}

void MaggsA3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MaggsA3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif


void MaggsA3AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    AudioBuffer<float> wetBuffer(getTotalNumInputChannels(), buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);
    
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto* wetData = wetBuffer.getWritePointer(channel);
        
        
        //Implement Pan Fucntion
        //Pan pot / range + 0.5
        auto channelAmplitude = *panParameter /200.0f + 0.5f;
        auto channelSelector = *panY;
        auto outputValue = *outputMenu;
        
        auto frontBack = *panY / 200.0f + 0.5f;
        auto front = 1 - frontBack ;
        auto back = frontBack;
        float mid;
        float frontMid;
        float backMid;
        
        
        //make mid selector scale to 100 in the middle
        if(channelSelector < 0 && channelSelector > -100 )
        {
            mid = (back + back);
        }
        
        else if (channelSelector < 100 && channelSelector > 0)
        {
            mid = front * 2;
        }
        
        if(channelSelector < -50 && channelSelector > -100 )
        {
            frontMid = (back + back + back + back);
        }
        
        else if (channelSelector < 0 && channelSelector > -50)
        {
            frontMid = front;
        }
        
        if(channelSelector > 50 && channelSelector < 100 )
        {
            backMid = (back + back + back + back);
        }
        
        else if (channelSelector < 100 && channelSelector > 50)
        {
            backMid = front * 4;
        }
        
        auto frontL = channel == 0;
        auto frontR = channel == 1;
        auto centre = channel == 2;
        
        auto lSide = channel == 3;
        auto rSide = channel == 4;
        auto lMidSide = channel == 5;
        auto rMidSide = channel == 6;
        auto lRear = channel == 7;
        auto rRear = channel == 8;
        
  
        
        
        
        
        
        for(int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            
            //Stero
            
            if (outputValue == 0.0)
            {
                if (channelSelector > -100 && channelSelector < 100)
                {
                    if (channel == 0 )
                    {
                        
                        //Square Root panning
                        frontL = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude));
                        
                    }
                    else if (channel == 1 )
                    {
                        //Square Root panning
                        frontR = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude));
                        
                        
                    }
                }
                
                //silence all channels when not in bounds
                else
                {
                    channelData[sample] = 0;
                }
                
            }
            
            // Quad
            
            else if (outputValue == 1.0)
            {
                if(channelSelector <0.0f && channelSelector > -100.0f)
                {
                    if (channel == 0 )
                    {
                        
                        //Square Root panning
                       frontL = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude));
                        
                    }
                    else if (channel == 1 )
                    {
                        //Square Root panning
                        frontR  = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude));
                        
                    }
                    
                    //silence rear speakers
                    if (channel == 3)
                    {
                       lSide = channelData[sample] = 0;
                    }
                    else if(channel == 4)
                    {
                        rSide = channelData[sample] = 0;
                    }
                }
                
                else if(channelSelector > 0.0f && channelSelector < 100.0f)
                {
                    
                    if (channel == 3 )
                    {
                        //Square Root panning
                       lSide = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude));
                        
                    }
                    
                    else if (channel == 4 )
                    {
                        //Square Root panning
                       rSide = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude));
                    }
                    //silence front speakers
                    
                    if (channel == 0)
                    {
                       frontL = channelData[sample] = 0;
                    }
                    else if(channel == 1)
                    {
                        frontR = channelData[sample] = 0;
                    }
                }
                //silence all channels when not in bounds
                else
                {
                    channelData[sample] = 0;
                }
                
            }
            
            
            //5.1
            
            else if (outputValue == 2.0)
            {
                
                if(channel == 0)
                {
                   frontL = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * front);
                }
                
                else if (channel == 1)
                {
                    frontR = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * front);
                }
                
                else if (channel == 2)
                {
                    centre = channelData[sample] = wetData[sample] * (channelAmplitude * front);
                }
                
                else if (channel == 3)
                {
                   lSide = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * back);
                }
                
                else if (channel == 4)
                {
                    rSide = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * back);
                }
                
                
            }
            
            //7.1
            
            else if (outputValue == 3.0)
            {
                
                if(channel == 0)
                {
                    frontL = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * front);
                }
                
                else if (channel == 1)
                {
                    frontR = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * front);
                }
                
                else if (channel == 2)
                {
                    centre = channelData[sample] = wetData[sample] * (channelAmplitude * front);
                }
                
                else if (channel == 3)
                {
                    lSide = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * mid);
                }
                
                else if (channel == 4)
                {
                    rSide = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * mid);
                }
                
                else if (channel == 5)
                {
                    lMidSide = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * back);
                }
                
                else if (channel == 6)
                {
                    rMidSide = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * back);
                }
            }
            
            //Octagonal
            
            else if (outputValue == 4.0)
            {
                
                if(channelSelector <-50.0f && channelSelector > -100.0f)
                {
                    if (channel == 0 )
                    {
                        
                        //Square Root panning
                        channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude));
                        
                    }
                    else if (channel == 1 )
                    {
                        //Square Root panning
                        channelData[sample] = wetData[sample] * (sqrt(channelAmplitude));
                        
                    }
                    //silence all other speakers
                    
                    if (channel == 2)
                    {
                        channelData[sample] = 0;
                    }
                    else if(channel == 3)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 4)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 5)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 6)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 7)
                    {
                        channelData[sample] = 0;
                    }
                }
                
                else if(channelSelector < 0.0f && channelSelector > -50.0f)
                {
                    
                    if (channel == 2 )
                    {
                        //Square Root panning
                        channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude));
                        
                    }
                    
                    else if (channel == 3 )
                    {
                        //Square Root panning
                        channelData[sample] = wetData[sample] * (sqrt(channelAmplitude));
                    }
                    //silence all other speakers
                    
                    if (channel == 0)
                    {
                        channelData[sample] = 0;
                    }
                    else if(channel == 1)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 4)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 5)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 6)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 7)
                    {
                        channelData[sample] = 0;
                    }
                }
                
                else if(channelSelector > 0.0f && channelSelector < 50.0f)
                {
                    
                    if (channel == 4 )
                    {
                        //Square Root panning
                        channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude));
                        
                    }
                    
                    else if (channel == 5 )
                    {
                        //Square Root panning
                        channelData[sample] = wetData[sample] * (sqrt(channelAmplitude));
                    }
                    //silence all other speakers
                    
                    
                    if (channel == 0)
                    {
                        channelData[sample] = 0;
                    }
                    else if(channel == 1)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 2)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 3)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 6)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 7)
                    {
                        channelData[sample] = 0;
                    }
                }
                
                else if(channelSelector > 50.0f && channelSelector < 100.0f)
                {
                    
                    if (channel == 6 )
                    {
                        //Square Root panning
                        channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude));
                        
                    }
                    
                    else if (channel == 7 )
                    {
                        //Square Root panning
                        channelData[sample] = wetData[sample] * (sqrt(channelAmplitude));
                    }
                    
                    //silence all other speakers
                    
                    if (channel == 0)
                    {
                        channelData[sample] = 0;
                    }
                    else if(channel == 1)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 2)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 3)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 4)
                    {
                        channelData[sample] = 0;
                    }
                    
                    else if(channel == 5)
                    {
                        channelData[sample] = 0;
                    }
                }
                //silence all channels when not in bounds
                else
                {
                    channelData[sample] = 0;
                }
            }
            
            // 9.1
            
            else if (outputValue == 5.0)
            {
                channelData[sample] = 0;
                
                /*if(channel == 0)
                {
                    frontL = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * front);
                }
                
                else if (channel == 1)
                {
                    frontR = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * front);
                }
                
                else if (channel == 2)
                {
                    centre = channelData[sample] = wetData[sample] * (channelAmplitude * front);
                }
                
                else if (channel == 3)
                {
                    lSide = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * frontMid);
                }
                
                else if (channel == 4)
                {
                    rSide = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * frontMid);
                }
                
                else if (channel == 5)
                {
                    lMidSide = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * backMid);
                }
                
                else if (channel == 6)
                {
                    rMidSide = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * backMid);
                }
                
                else if (channel == 7)
                {
                    lRear = channelData[sample] = wetData[sample] * (sqrt(1-channelAmplitude) * back);
                }
                
                else if (channel == 8)
                {
                    rRear = channelData[sample] = wetData[sample] * (sqrt(channelAmplitude) * back);
                }*/
                
            }
            
        }
    }
    
}





//==============================================================================
bool MaggsA3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MaggsA3AudioProcessor::createEditor()
{
    return new MaggsA3AudioProcessorEditor (*this, parameters);
}

//==============================================================================
void MaggsA3AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MaggsA3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(ValueTree::fromXml(*xmlState));
        }
    }
}




//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MaggsA3AudioProcessor();
}
