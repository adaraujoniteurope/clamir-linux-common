package com.profinet;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Unmarshaller;
import java.io.File;

public class Main {
    public static void main(String[] args) {
        try {
            File file = new File("src/main/resources/example.xml");

            JAXBContext jaxbContext = JAXBContext.newInstance(ISO15745Profile.class);
            Unmarshaller unmarshaller = jaxbContext.createUnmarshaller();
            ISO15745Profile profile = (ISO15745Profile) unmarshaller.unmarshal(file);

            // Print or use the populated profile object
            System.out.println(profile.getProfileHeader().getProfileIdentification());
        } catch (JAXBException e) {
            e.printStackTrace();
        }
    }
}