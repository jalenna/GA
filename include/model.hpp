#pragma once

class NN
{
private:
    /* data */
public:
    NN(/* args */);

    void InitModel(int inputSize, int numLayers, int numParameters, int outputSize);
    void Predict();

    ~NN();
};

NN::NN(/* args */)
{
}

NN::~NN()
{
}
