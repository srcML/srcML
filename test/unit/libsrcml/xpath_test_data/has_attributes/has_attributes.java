@Entity
public class Vehicle {

    @Persistent
    protected String vehicleName = null;
    
    @Getter
    public String getVehicleName() { }

    public void setVehicleName(@Optional vehicleName) {
    }

    public List addVehicleNameToList(List names) {

        @Optional
        List localNames = names;
    }
}


@interface /* This isn't thing that should be matched */ something {
}
