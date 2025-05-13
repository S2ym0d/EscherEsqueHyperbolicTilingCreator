#include "TilingSettings.h"
#include <fstream>
#include <functional>
#include "HelpfulStringFunctions.h"
#include <iostream>
#include <iomanip>

void LoadPQRValues(TilingSettings& settings, const std::string& line)
{
    std::string pqrValuesStr = RemoveAllOccurrencesOfSubstring(RemoveAllOccurrencesOfSubstring(line, "("), ")");

    auto splitedPQRValues = SplitString(pqrValuesStr, ",");

    if (splitedPQRValues.size() < 3)
    {
        std::cout << "Not enough arguments for p, q, r values." << std::endl;
        return;
    }

    if (splitedPQRValues.size() > 3)
    {
        std::cout << "Too many arguments for p, q, r values." << std::endl;
        return;
    }

    std::array<long long, 3> pqr;
    size_t idx = 0;

    idx = 0;
    {
        std::string argument = TrimLeadingAndTrailingSpaces(splitedPQRValues.at(idx));

        if (argument == "oo")
        {
            pqr.at(idx) = -1;
        }
        else
        {
            try
            {
                long long val{ std::stoll(argument) };
                pqr.at(idx) = val;
            }
            catch (std::invalid_argument const& ex)
            {
                std::cout << "Invalid argument: " << argument << ", at p, q, r values setting." << std::endl;
                return;
            }
            catch (std::out_of_range const& ex)
            {
                std::cout << "Argument out of range: " << argument << ", at p, q, r values setting." << std::endl;
                return;
            }
        }
    }

    idx = 1;
    {
        std::string argument = TrimLeadingAndTrailingSpaces(splitedPQRValues.at(idx));

        if (argument == "oo")
        {
            pqr.at(idx) = -1;
        }
        else
        {
            try
            {
                long long val{ std::stoll(argument) };
                pqr.at(idx) = val;
            }
            catch (std::invalid_argument const& ex)
            {
                std::cout << "Invalid argument: " << argument << ", at p, q, r values setting." << std::endl;
                return;
            }
            catch (std::out_of_range const& ex)
            {
                std::cout << "Argument out of range: " << argument << ", at p, q, r values setting." << std::endl;
                return;
            }
        }
    }

    idx = 2;
    {
        std::string argument = TrimLeadingAndTrailingSpaces(splitedPQRValues.at(idx));

        if (argument == "oo")
        {
            pqr.at(idx) = -1;
        }
        else
        {
            try
            {
                long long val{ std::stoll(argument) };
                pqr.at(idx) = val;
            }
            catch (std::invalid_argument const& ex)
            {
                std::cout << "Invalid argument: " << argument << ", at p, q, r values setting." << std::endl;
                return;
            }
            catch (std::out_of_range const& ex)
            {
                std::cout << "Argument out of range: " << argument << ", at p, q, r values setting." << std::endl;
                return;
            }
        }
    }

    settings.pqrValues = pqr;
}

void LoadSeparatingSides(TilingSettings& settings, const std::string& line)
{
    std::array<bool, 3> separatingSides = { false, false, false };

    if (line.find('a') != std::string::npos)
    {
        separatingSides.at(0) = true;
    }

    if (line.find('b') != std::string::npos)
    {
        separatingSides.at(1) = true;
    }

    if (line.find('c') != std::string::npos)
    {
        separatingSides.at(2) = true;
    }

    if (separatingSides.at(0) || separatingSides.at(1) || separatingSides.at(2))
    {
        settings.separatingSide = separatingSides;
    }
    else
    {
        std::cout << "At least one side has to be separating." << std::endl;
    }
}

void LoadInformationAboutDifferentImagesForEvenAndOddTiles(TilingSettings& settings, const std::string& line)
{
    if (line.find("false") != std::string::npos)
    {
        settings.oddEvenImageTiling = false;
        return;
    }

    if (line.find("true") != std::string::npos)
    {
        settings.oddEvenImageTiling = true;
        return;
    }

    std::cout << "Invalid argument in option about different images for even and odd tiles." << std::endl;
}

void LoadEvenTilesImageFilePath(TilingSettings& settings, const std::string& line)
{
    settings.evenImageFilePath = TrimLeadingAndTrailingSpaces(line);
}

void LoadOddTilesImageFilePath(TilingSettings& settings, const std::string& line)
{
    settings.oddImageFilePath = TrimLeadingAndTrailingSpaces(line);
}

std::pair<double, bool> LoadAngle(const std::string& line)
{
    try
    {
        double val{ std::stod(line) };
        return { val, true };
    }
    catch (std::invalid_argument const& ex)
    {
        std::cout << "Invalid argument: " << line;
        return { 0.0, false };
    }
    catch (std::out_of_range const& ex)
    {
        std::cout << "Argument out of range: " << line;
        return { 0.0, false };
    }
}

std::pair<HyperboloidModelPoint, bool> LoadTranslationHyperboloidVector(const std::string& line)
{
    std::string vectorStr = RemoveAllOccurrencesOfSubstring(RemoveAllOccurrencesOfSubstring(line, "("), ")");

    auto splitedVectorValues = SplitString(vectorStr, ",");

    if (splitedVectorValues.size() < 2)
    {
        std::cout << "Not enough arguments for translation vector";
        return { HyperboloidModelPoint(), false };
    }

    if (splitedVectorValues.size() > 3)
    {
        std::cout << "Too many arguments for translation vector" << std::endl;
        return { HyperboloidModelPoint(), false };
    }

    std::array<double, 3> vectorArrayOfArguments;
    long long shift = splitedVectorValues.size() == 3 ? 0 : -1;

    long long idx = 0;

    idx = 1;
    {
        try
        {
            double val{ std::stod(splitedVectorValues.at(idx + shift)) };
            vectorArrayOfArguments.at(idx) = val;
        }
        catch (std::invalid_argument const& ex)
        {
            std::cout << "Invalid argument: " << splitedVectorValues.at(idx + shift);
            return { HyperboloidModelPoint(), false };
        }
        catch (std::out_of_range const& ex)
        {
            std::cout << "Argument out of range: " << splitedVectorValues.at(idx + shift);
            return { HyperboloidModelPoint(), false };
        }
    }

    idx = 2;
    {
        try
        {
            double val{ std::stod(splitedVectorValues.at(idx + shift)) };
            vectorArrayOfArguments.at(idx) = val;
        }
        catch (std::invalid_argument const& ex)
        {
            std::cout << "Invalid argument: " << splitedVectorValues.at(idx + shift);
            return { HyperboloidModelPoint(), false };
        }
        catch (std::out_of_range const& ex)
        {
            std::cout << "Argument out of range: " << splitedVectorValues.at(idx + shift);
            return { HyperboloidModelPoint(), false };
        }
    }

    return { HyperboloidModelPoint(vectorArrayOfArguments), true };
}

const std::array< std::pair<std::string, HyperbolicModel>, 6> modelNames =
{
    std::pair<std::string, HyperbolicModel>({"PoincareDisk", HyperbolicModel::PoincareDisk}),
    {"BeltramiKleinDisk",HyperbolicModel::BeltramiKleinDisk},
    {"PoincareHalfPlane",HyperbolicModel::PoincareHalfPlane},
    {"Gans",HyperbolicModel::Gans},
    {"Band",HyperbolicModel::Band},
    {"AzimuthalEquidistant",HyperbolicModel::AzimuthalEquidistant}
};

std::pair<HyperbolicModel, bool> LoadHyperbolicModel(const std::string& line)
{
    auto argument = TrimLeadingAndTrailingSpaces(line);

    for (size_t i{ 0 }; i < modelNames.size(); i++)
    {
        if (argument == modelNames.at(i).first)
        {
            return { modelNames.at(i).second, true };
        }
    }

    std::cout << "Invalid argument: " << argument;
    return { PoincareDisk, false };
}

void LoadSourceImageLocalRotationAngle(TilingSettings& settings, const std::string& line)
{
    auto angle = LoadAngle(line);

    if (angle.second)
    {
        settings.sourceImageLocalRotation = angle.first;
    }
    else
    {
        std::cout << ", at source image local rotation angle setting." << std::endl;
    }
}

void LoadSourceImageTranslationVector(TilingSettings& settings, const std::string& line)
{
    auto vector = LoadTranslationHyperboloidVector(line);

    if (vector.second)
    {
        settings.sourceImageTranslation = vector.first;
    }
    else
    {
        std::cout << ", at source image translation hyperboloid vector setting." << std::endl;
    }
}

void LoadSourceImageGlobalRotationAngle(TilingSettings& settings, const std::string& line)
{
    auto angle = LoadAngle(line);

    if (angle.second)
    {
        settings.sourceImageGlobalRotation = angle.first;
    }
    else
    {
        std::cout << ", at source image global rotation angle setting." << std::endl;
    }
}

void LoadSourceImageHyperbolicModel(TilingSettings& settings, const std::string& line)
{
    auto model = LoadHyperbolicModel(line);

    if (model.second)
    {
        settings.sourceImageModel = model.first;
    }
    else
    {
        std::cout << ", at source image hyperbolic model setting." << std::endl;
    }
}

void LoadTilingImageLocalRotationAngle(TilingSettings& settings, const std::string& line)
{
    auto angle = LoadAngle(line);

    if (angle.second)
    {
        settings.tilingImageLocalRotation = angle.first;
    }
    else
    {
        std::cout << ", at tiling image local rotation angle setting." << std::endl;
    }
}

void LoadTilingImageTranslationVector(TilingSettings& settings, const std::string& line)
{
    auto vector = LoadTranslationHyperboloidVector(line);

    if (vector.second)
    {
        settings.tilingImageTranslation = vector.first;
    }
    else
    {
        std::cout << ", at tiling image translation hyperboloid vector setting." << std::endl;
    }
}

void LoadTilingImageGlobalRotationAngle(TilingSettings& settings, const std::string& line)
{
    auto angle = LoadAngle(line);

    if (angle.second)
    {
        settings.tilingImageGlobalRotation = angle.first;
    }
    else
    {
        std::cout << ", at tiling image global rotation angle setting." << std::endl;
    }
}

void LoadTilingImageHyperbolicModel(TilingSettings& settings, const std::string& line)
{
    auto model = LoadHyperbolicModel(line);

    if (model.second)
    {
        settings.tilingImageModel = model.first;
    }
    else
    {
        std::cout << ", at tiling image hyperbolic model setting." << std::endl;
    }
}

const std::array< std::pair<std::string, TilingAlgorithm>, 2> tilingAlgorithmsNames =
{
    std::pair<std::string, TilingAlgorithm>({"fast", TilingAlgorithm::fast}),
    {"tileInternallyConsistent",TilingAlgorithm::tileInternallyConsistent}
};

void LoadTilingAlgorithmType(TilingSettings& settings, const std::string& line)
{
    auto argument = TrimLeadingAndTrailingSpaces(line);

    for (size_t i{ 0 }; i < tilingAlgorithmsNames.size(); i++)
    {
        if (argument == tilingAlgorithmsNames.at(i).first)
        {
            settings.tilingAlgorithm = tilingAlgorithmsNames.at(i).second;
            return;
        }
    }

    std::cout << "Invalid argument: " << argument << ", at tiling algorithm setting." << std::endl;
}

const std::array< std::pair<std::string, CenteringType>, 2> centeringTypeNames =
{
    std::pair<std::string, CenteringType>({"vertexCentered", CenteringType::vertexCentered}),
    {"footOfAltitudeCentered",CenteringType::footOfAltitudeCentered}
};

void LoadCenteringType(TilingSettings& settings, const std::string& line)
{
    auto argument = TrimLeadingAndTrailingSpaces(line);

    for (size_t i{ 0 }; i < centeringTypeNames.size(); i++)
    {
        if (argument == centeringTypeNames.at(i).first)
        {
            settings.centeringType = centeringTypeNames.at(i).second;
            return;
        }
    }

    std::cout << "Invalid argument: " << argument << ", at centering type setting." << std::endl;
}

const std::array<std::pair<std::string, std::function<void(TilingSettings&, const std::string&)>>, 15> settingsNames =
{
    std::pair<std::string, std::function<void(TilingSettings&, const std::string&)>>({"Schwarz triangle (p, q, r) values:", LoadPQRValues}),
    {"separating sides:", LoadSeparatingSides},

    {"different images for even and odd tiles:", LoadInformationAboutDifferentImagesForEvenAndOddTiles},
    {"even tiles image path:", LoadEvenTilesImageFilePath},
    {"odd tiles image path:", LoadOddTilesImageFilePath},

    {"source image local rotation angle:", LoadSourceImageLocalRotationAngle},
    {"source image translation hyperboloid vector:", LoadSourceImageTranslationVector},
    {"source image global rotation angle:", LoadSourceImageGlobalRotationAngle},
    {"source image hyperbolic model:", LoadSourceImageHyperbolicModel},

    {"tiling image local rotation angle:", LoadTilingImageLocalRotationAngle},
    {"tiling image translation hyperboloid vector:", LoadTilingImageTranslationVector},
    {"tiling image global rotation angle:", LoadTilingImageGlobalRotationAngle},
    {"tiling image hyperbolic model:", LoadTilingImageHyperbolicModel},

    {"tiling algorithm:", LoadTilingAlgorithmType},
    {"centering type:", LoadCenteringType}
};

std::string ModelNamesAsOneString()
{
    std::string namesStr = " ";

    for (const auto& name : modelNames)
    {
        namesStr += name.first + ", ";
    }

    namesStr = namesStr.substr(0, namesStr.size() - 2);

    return namesStr;
}

std::string TilingAlgorithmsNamesAsOneString()
{
    std::string namesStr = " ";

    for (const auto& name : tilingAlgorithmsNames)
    {
        namesStr += name.first + ", ";
    }

    namesStr = namesStr.substr(0, namesStr.size() - 2);

    return namesStr;
}

std::string CenteringTypeNamesAsOneString()
{
    std::string namesStr = " ";

    for (const auto& name : centeringTypeNames)
    {
        namesStr += name.first + ", ";
    }

    namesStr = namesStr.substr(0, namesStr.size() - 2);

    return namesStr;
}

const std::array<std::string, settingsNames.size()> settingsComments =
{
    "2 <= p, q, r <= oo, 1/p + 1/q + 1/r < 1, e.g. (7, 3, 2)",
    "sides names are a, b and c, e.g. b - only side b is separating, ac - both side a and side c are separating sides",

    "true or false",
    "image path",
    "image path",

    "real value, e.g. 0.0",
    "hyperboloid model point (w, x, y) or Gans model point (x, y), e.g. (3.0, 2.0, -2.0)",
    "real value, e.g. 0.0",
    "model name:" + ModelNamesAsOneString(),

    "real value, e.g. 0.0",
    "hyperboloid model point (w, x, y) or Gans model point (x, y), e.g. (3.0, 2.0, -2.0)",
    "real value, e.g. 0.0",
    "model name:" + ModelNamesAsOneString(),

    "tiling algorithm:" + TilingAlgorithmsNamesAsOneString(),
    "centering type:" + CenteringTypeNamesAsOneString()
};

const std::array<std::string, settingsNames.size()> settingsDefaultValues
{
    "(7, 3, 2) ",
    "abc ",

    "false ",
    "image0.png ",
    "image1.png ",

    "0.0 ",
    "(1.0, 0.0 ,0.0) ",
    "0.0 ",
    "PoincareDisk ",

    "0.0 ",
    "(1.0, 0.0 ,0.0) ",
    "0.0 ",
    "PoincareDisk ",

    "fast ",
    "vertexCentered "
};

std::pair<TilingSettings, bool> LoadTilingSettings(const std::string& settingsFilePath)
{
    std::ifstream settingsFile;

    settingsFile.open(settingsFilePath);

    if (!settingsFile.is_open())
    {
        std::cout << "Settings file " << settingsFilePath << " couldn't be opened." << std::endl;

        return { TilingSettings(), false };
    }

    TilingSettings settings;

    std::string line;

    while (std::getline(settingsFile, line))
    {
        line = SubstringBeforeFirstOccurrenceOfSeparator(line, "#");
        
        for (const auto& setting : settingsNames)
        {
            if (line.find(setting.first) != std::string::npos)
            {
                line = SubstringAfterFirstOccurrenceOfSeparator(line, setting.first);

                setting.second(settings, line);

                break;
            }
        }
    }

    settingsFile.close();

    return { settings, true };
}

size_t MaxSettingsNamesSize()
{
    size_t max = 0;

    for (const auto& name : settingsNames)
    {
        if (name.first.size() > max)
        {
            max = name.first.size();
        }
    }

    return max;
}

size_t MaxSettingsDefaultValuesNamesSize()
{
    size_t max = 0;

    for (const auto& name : settingsDefaultValues)
    {
        if (name.size() > max)
        {
            max = name.size();
        }
    }

    return max;
}

void MakeTilingSettingsFile()
{
    const std::string filePath = "tilingSettings.txt";
    const std::string commentSymbol = "# ";

    const size_t maxSettingNameSize = MaxSettingsNamesSize();
    const size_t maxDefaultSettingValueSize = MaxSettingsDefaultValuesNamesSize();

    std::ofstream settingsFile;

    settingsFile.open(filePath);

    if (!settingsFile.is_open())
    {
        std::cout << "Settings file " << filePath << " couldn't be opened." << std::endl;

        return;
    }

    for (size_t i{ 0 }; i < settingsNames.size(); i++)
    {
        settingsFile << std::setw(maxSettingNameSize + 1) << std::left << settingsNames.at(i).first << std::setw(maxDefaultSettingValueSize + 1) << settingsDefaultValues.at(i) << commentSymbol << settingsComments.at(i) << '\n';
    }

    settingsFile.close();
}
