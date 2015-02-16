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

        int chorus = 0.025 + sin(0.25 * 2 * PI * t) * 0.005 seconds;
        int rdloc = (wrloc + QUEUESIZE - chorus) % QUEUESIZE;

        audio[0] = audio[0]/2 + queue[rdloc++]/2;
        audio[1] = audio[1]/2 + queue[rdloc]/2;

        ProcessWriteFrame(audio);

        // The progress control
        if(!ProcessProgress(double(i) / SampleFrames()))
            break;
    }


    // Call to close the generator output
    ProcessEnd();
}
