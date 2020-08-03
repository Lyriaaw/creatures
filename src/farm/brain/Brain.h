//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BRAIN_H
#define CREATURES_BRAIN_H


#include <vector>
#include "neurons/InputNeuron.h"
#include "neurons/OutputNeuron.h"
#include "Link.h"

class Brain {

public:
    const std::vector<Neuron *> &getNeurons() const;

    void addNeuron(Neuron * neuron);
    void addInputNeuron(InputNeuron * neuron);
    void addOutputNeuron(OutputNeuron * neuron);

    void addLink(Link * link);


    const std::vector<InputNeuron *> &getInputNeurons() const;
    const std::vector<OutputNeuron *> &getOutputNeurons() const;
    const std::vector<Link *> &getLinks() const;

    const std::vector<std::vector<Link *>> &getLinksGrid() const;

    const std::vector<int> &getLinesNumber() const;

    void generateLinkGrid();


private:
    std::vector<Neuron *> neurons;
    std::vector<InputNeuron *> inputNeurons;
    std::vector<OutputNeuron *> outputNeurons;

    std::vector<Link *> links;
    std::vector<std::vector<Link *>> linksGrid;

    std::vector<int> linesNumber;


};


#endif //CREATURES_BRAIN_H
