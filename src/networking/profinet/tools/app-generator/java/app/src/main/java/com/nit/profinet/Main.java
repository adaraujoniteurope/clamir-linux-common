package com.nit.profinet;

import org.apache.commons.text.WordUtils;

import com.nit.profinet.*;
import com.nit.profinet.ISO15745Profile.*;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import java.io.File;

public class Main {
    public static void main(String[] args) {
        try {
            File file = new File("src/main/resources/example.xml");

            // Create JAXBContext for your model class
            JAXBContext jaxbContext = JAXBContext.newInstance(ISO15745Profile.class);

            // Create an unmarshaller
            Unmarshaller unmarshaller = jaxbContext.createUnmarshaller();
            Marshaller marshaller = jaxbContext.createMarshaller();

            
            marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);

            // Unmarshal the XML into your object model
            var profile = new ISO15745Profile();
            var profileHeader = new ProfileHeader();

            profile.setProfileHeader(profileHeader);

            marshaller.marshal(profile, file);

            // Output some information from the unmarshalled object
            // System.out.println("Profile Identification: " + profile.getProfileHeader().getProfileIdentification());
        } catch (JAXBException e) {
            e.printStackTrace();
        }
    }
}