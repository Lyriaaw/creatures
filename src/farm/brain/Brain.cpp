//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "Brain.h"

const std::vector<Neuron *> &Brain::getNeurons() const {
    return neurons;
}

void Brain::addNeuron(Neuron * neuron){
    this->neurons.push_back(neuron);


    int index(-1);
    bool found(false);
    for (int it = 0; it < linesNumber.size(); it++) {
        if (linesNumber.at(it) < neuron->getX()) {
            index = it;
        }
        if (linesNumber.at(it) == neuron->getX()) {
            found = true;
        }

    }

    if (!found) {
        linesNumber.insert(linesNumber.begin() + index + 1, neuron->getX());
    }
}

void Brain::addLink(Link *link){
    this->links.push_back(link);
}

void Brain::removeLink(int inputGenerationNumber, int outputGenerationNumber) {

    int foundIndex = -1;
    for (int it = 0; it < links.size(); it++) {
        Link * currentLink = links.at(it);

        if (currentLink->getInput()->getGenerationNumber() == inputGenerationNumber && currentLink->getOutput()->getGenerationNumber() == outputGenerationNumber) {
            foundIndex = it;
        }
    }

    if (foundIndex == -1) {
        std::cout << "Error while removing link. Link not found" << std::endl;
        return;
    }

    links.erase(links.begin() + foundIndex);
}

void Brain::generateLinkGrid(){

    for (int lineNumberIndex = 0; lineNumberIndex < linesNumber.size(); lineNumberIndex++) {
        std::vector<Link *> currentLinksColumn;

        for (int it = 0; it < links.size(); it++) {
            if (links.at(it)->getInput()->getX() == linesNumber.at(lineNumberIndex)) {
                currentLinksColumn.emplace_back(links.at(it));
            }
        }

        linksGrid.emplace_back(currentLinksColumn);
    }
}


void Brain::addInputNeuron(InputNeuron * neuron){
    int inputNeuronCount = inputNeurons.size();

    neuron->setX(0.f);
    neuron->setY(10.f * float(inputNeuronCount));

    this->inputNeurons.push_back(neuron);
    this->addNeuron(neuron);
}


void Brain::addOutputNeuron(OutputNeuron * neuron){
    int outputNeuronCount = outputNeurons.size();

    neuron->setX(100.f);
    neuron->setY(10.f * float(outputNeuronCount));

    this->outputNeurons.push_back(neuron);
    this->addNeuron(neuron);
}




const std::vector<InputNeuron *> &Brain::getInputNeurons() const {
    return inputNeurons;
}

const std::vector<OutputNeuron *> &Brain::getOutputNeurons() const {
    return outputNeurons;
}

const std::vector<Link *> &Brain::getLinks() const {
    return links;
}

const std::vector<int> &Brain::getLinesNumber() const {
    return linesNumber;
}

const std::vector<std::vector<Link *>> &Brain::getLinksGrid() const {
    return linksGrid;
}

