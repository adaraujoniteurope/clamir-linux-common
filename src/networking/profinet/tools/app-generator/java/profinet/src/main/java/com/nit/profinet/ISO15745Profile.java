package com.nit.profinet;

import java.util.List;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "ISO15745Profile")
public class ISO15745Profile {
    private ProfileHeader profileHeader;
    // private ProfileBody profileBody;

    // Getters and setters
    public ProfileHeader getProfileHeader() {
        return profileHeader;
    }

    @XmlElement(name = "ProfileHeader")
    public void setProfileHeader(ProfileHeader profileHeader) {
        this.profileHeader = profileHeader;
    }
    
    public static class ProfileHeader {
        private String profileIdentification;

        // Getters and setters
        public String getProfileIdentification() {
            return profileIdentification;
        }

        @XmlElement
        public void setProfileIdentification(String profileIdentification) {
            this.profileIdentification = profileIdentification;
        }
    }
}