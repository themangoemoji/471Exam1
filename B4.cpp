AudioProcessDoc::OnProcessDelay()
{
    // Call to open the processing output
    if(!ProcessBegin())
        return;

    short audio[2];

    const int QUEUESIZE = 200000;
    const double DELAY = 1.0;

    std::vector<short> queue;
    queue.resize(QUEUESIZE);

    int wrloc = 0;

    double time = 0;

    for(int i=0;  i<SampleFrames();  i++, time += 1./SampleRate())
    {                 
        ProcessReadFrame(audio);

        wrloc = (wrloc + 2) % QUEUESIZE;
        queue[wrloc] = audio[0];
        queue[wrloc+1] = audio[1];

        int flange = 0.006 + sin(0.25 * 2 * PI * t) * 0.004;

        double diff = 3.14 * sin(time * 2 * M_PI * freq2); 
        short sample = short(amplitude *  sin(time * 2 * M_PI * freq1 + diff));

        audio[0] = sample;
        audio[1] = sample;

        ProcessWriteFrame(audio);

        // The progress control
        if(!ProcessProgress(double(i) / SampleFrames()))
            break;
    }


    // Call to close the generator output
    ProcessEnd();
}
